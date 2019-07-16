/**************************************************************************
 *
 *  pngin.cpp
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PNG 形式入力クラス
 *
 *  last update: 2018.12.23
 *
 */
#include <cstdio>
#include <iostream>
#include <fstream>

extern "C" {
#include <png.h>
}

#include "pngio.h"



#define PNG_CHECK_BYTES 8

namespace
{

// PNGファイルかどうか調べる
bool isPNG__(const std::filesystem::path& path)
{
  using namespace std;

  ifstream ifs(path, ios::in | ios::binary);
  if (!ifs)
    return false;

  unsigned char buf[PNG_CHECK_BYTES];
  if (!ifs.read((char*)buf, sizeof(buf)))
    return false;
  //return png_check_sig(buf, PNG_CHECK_BYTES) == 0 ? false : true;
  return png_sig_cmp(buf, 0, PNG_CHECK_BYTES) ? false : true;
}


// PNGのデータ形式を特定する
struct PNGColorBitType_
{
  int cbits;
  int ctype;
  int type;
  int bits;
};

enum { PNG_NUL__, PNG_RGB__, PNG_GRY__, PNG_PAL__ };

PNGColorBitType_ pngtype_G[]
 =
{
  {  1, PNG_COLOR_TYPE_PALETTE,  PNG_PAL__,  1 },
  {  2, PNG_COLOR_TYPE_PALETTE,  PNG_PAL__,  2 },
  {  4, PNG_COLOR_TYPE_PALETTE,  PNG_PAL__,  4 },
  {  8, PNG_COLOR_TYPE_PALETTE,  PNG_PAL__,  8 },
  {  8, PNG_COLOR_TYPE_RGB,      PNG_RGB__, 24 },
  { 16, PNG_COLOR_TYPE_RGB,      PNG_RGB__, 48 },
  {  1, PNG_COLOR_TYPE_GRAY,     PNG_GRY__,  1 },
  {  2, PNG_COLOR_TYPE_GRAY,     PNG_GRY__,  2 },
  {  4, PNG_COLOR_TYPE_GRAY,     PNG_GRY__,  4 },
  {  8, PNG_COLOR_TYPE_GRAY,     PNG_GRY__,  8 },
  { 16, PNG_COLOR_TYPE_GRAY,     PNG_GRY__, 16 },
  { 0, -1, PNG_NUL__, 0 }
};


/* color format のチェック */
void checkColorFormat__(bool* alpha, int* type, int* obits, int bits, int ctype)
{
  *alpha = false;

  *type = PNG_NUL__;
  *obits = -1;

  if (ctype == PNG_COLOR_TYPE_RGB_ALPHA)
  {
    ctype = PNG_COLOR_TYPE_RGB;
    *alpha = true;
  }
  else if (ctype == PNG_COLOR_TYPE_GRAY_ALPHA)
  {
    ctype = PNG_COLOR_TYPE_GRAY;
    *alpha = true;
  }

  for (int i=0; pngtype_G[i].cbits != 0; i++)
  {
    if (bits == pngtype_G[i].cbits && ctype == pngtype_G[i].ctype)
    {
      *type = pngtype_G[i].type;
      *obits = pngtype_G[i].bits;
      break;
    }
  }
}


/*________________________________________________________
 *  
 *  ifstreamからの讀み込み
 *
 *  H23.4.24 update for libpng-1.5.x
 */
void pngRead__(png_structp png_ptr, png_bytep data, png_size_t length)
{
  using namespace std;
  ifstream* pifs = reinterpret_cast<ifstream*>(png_get_io_ptr(png_ptr));
  pifs->read(reinterpret_cast<char*>(data), length);

  // H23.4.24 coment out for libpng-1.5.x
  //((std::ifstream*)(png_ptr->io_ptr))->read((char*)data, length);
}


/* エラー時に呼び出される */
void pngError__(png_structp png_ptr, png_const_charp str)
{
  std::cerr << str << std::endl;
}


/* 讀み込み處理の初期化 */
bool
pngReadInit__(
  const std::filesystem::path& path, std::ifstream& ifs, png_structp& png_ptr,
  png_infop& info_ptr)
{
  using namespace std;

  // ファイルチェックとオープン
  if (!isPNG__(path))
    return false;

  ifs.open(path, ios::in | ios::binary);
  if (!ifs)
    return false;


  // PNG入力用構造体の生成
  png_ptr
    = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, nullptr, pngError__, pngError__);
  if (!png_ptr)
    return false;

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    return false;
  }

  // 入力関数の設定
  png_set_read_fn(png_ptr, (void *)&ifs, pngRead__);

  // 入力開始
  png_read_info(png_ptr, info_ptr);


  return true;
}



}//end of namespace NONAME


polymnia::Picture* polymnia::PngLoader::load(const std::filesystem::path& path)
{
  using namespace std;

  ifstream ifs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngReadInit__(path, ifs, png_ptr, info_ptr))
    return nullptr;

  png_uint_32 ww, hh;
  int bits, ctype, itype;
  int obits, type;
  bool alpha;

  png_get_IHDR(
    png_ptr, info_ptr, &ww, &hh, &bits, &ctype, &itype, nullptr, nullptr);
  checkColorFormat__(&alpha, &type, &obits, bits, ctype);

  png_set_strip_alpha(png_ptr);  // Alpha要素は無視する

  if (bits == 16)
    png_set_strip_16(png_ptr);     // 16bit画像は8bitに直す

  if (type == PNG_GRY__)
    png_set_gray_to_rgb(png_ptr);  // グレースケール -> TrueColor

  if (type == PNG_PAL__) {
    //if (obits == 16) {
    //  png_set_strip_16(png_ptr);
    //  obits = 8;
    //}

    png_set_palette_to_rgb(png_ptr);  // Indexed -> TrueColor
  }

  int n_pass = png_set_interlace_handling(png_ptr); // インターレス画像用

  png_read_update_info(png_ptr, info_ptr);

  Picture* pct = Picture::create(ww, hh);

  RgbColor* buf = pct->buffer();
  int o = pct->offset();
  for (int pass = 0; pass < n_pass; pass++)
    for (unsigned j = 0, y = 0;  y < hh;  y++, j += o)
    {
      png_read_row(png_ptr, (png_bytep)(buf + j), nullptr);
    }
  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

  return pct;
}


polymnia::PictureIndexed*
polymnia::IndexedPngLoader::load(const std::filesystem::path& path)
{
  using namespace std;
  using namespace themis;

  ifstream ifs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngReadInit__(path, ifs, png_ptr, info_ptr))
    return nullptr;


  png_uint_32 ww, hh;
  int bits, ctype, itype;
  int obits, type;
  bool alpha;

  png_get_IHDR(
    png_ptr, info_ptr, &ww, &hh, &bits, &ctype, &itype, nullptr, nullptr);
  checkColorFormat__(&alpha, &type, &obits, bits, ctype);

  if (type == PNG_RGB__)  // TrueColorは駄目
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return nullptr;
  }

  if (obits == 16)
  {
    png_set_strip_16(png_ptr); // 16bit画像は8bitに直す
    obits = 8;
  }
  if (alpha)
    png_set_strip_alpha(png_ptr); // Alpha要素は無視する
  if (type == PNG_PAL__ && obits < 8)
    png_set_packing(png_ptr); // PAL8bitに統一
  if (type == PNG_GRY__ && obits < 8)
    png_set_expand(png_ptr); // GRAY8bitに統一

  int n_pass = png_set_interlace_handling(png_ptr); // インターレス画像用

  png_read_update_info(png_ptr, info_ptr);



  PictureIndexed* pct = PictureIndexed::create(ww, hh);


  // Paletteのロード
  RgbColor* pal = pct->paletteBuffer();
  if (type == PNG_GRY__)
  {
    //グレイスケールなら自分で用意する
    int d = 255 / ((1<<obits) - 1);
    for (int i = 0, c = 0; c < 256; i++, c += d)
      pal[i] = RgbColor((UByte)c, (UByte)c, (UByte)c);
  }
  else if (type == PNG_PAL__)
  {
    int npal;
    png_colorp pngpal;
    png_get_PLTE(png_ptr, info_ptr, &pngpal, &npal);

    for (int i = 0; i < npal; i++)
      pal[i] = RgbColor(pngpal[i].red, pngpal[i].green, pngpal[i].blue);
  }


  UByte *buf = pct->buffer();
  int o = pct->offset();
  for (int pass = 0; pass < n_pass; pass++)
    for (unsigned j = 0, y = 0; y < hh; y++, j += o)
      png_read_row(png_ptr, nullptr, (png_bytep)&buf[j]);

  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

  return pct;
}


//eof
