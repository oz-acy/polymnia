/**************************************************************************
 *
 *  pngin.cpp
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PNG �`�����̓N���X
 *
 *  last update: 2012.3.1
 *
 *************************************************************************/

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

// PNG�t�@�C�����ǂ������ׂ�
bool isPNG__(const char* path)
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


// PNG�̃f�[�^�`������肷��
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


/* color format �̃`�F�b�N */
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
 *  ifstream�����椂ݍ���
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


/* �G���[���ɌĂяo����� */
void pngError__(png_structp png_ptr, png_const_charp str)
{
  std::cerr << str << std::endl;
}


/* 椂ݍ��ݙ|���̏����� */
bool
pngReadInit__(
  const char* path, std::ifstream& ifs, png_structp& png_ptr,
  png_infop& info_ptr)
{
  using namespace std;

  // �t�@�C���`�F�b�N�ƃI�[�v��
  if (!isPNG__(path))
    return false;

  ifs.open(path, ios::in | ios::binary);
  if (!ifs)
    return false;


  // PNG���͗p�\���̂̐���
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

  // ���͊֐��̐ݒ�
  png_set_read_fn(png_ptr, (void *)&ifs, pngRead__);

  // ���͊J�n
  png_read_info(png_ptr, info_ptr);


  return true;
}



}//end of namespace


polymnia::Picture* polymnia::PngLoader::load_(const char* path)
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

  png_set_strip_alpha(png_ptr);  // Alpha�v�f�͖�������

  if (bits == 16)
    png_set_strip_16(png_ptr);     // 16bit�摜��8bit�ɒ���

  if (type == PNG_GRY__)
    png_set_gray_to_rgb(png_ptr);  // �O���[�X�P�[�� -> TrueColor

  if (type == PNG_PAL__) {
    //if (obits == 16) {
    //  png_set_strip_16(png_ptr);
    //  obits = 8;
    //}

    png_set_palette_to_rgb(png_ptr);  // Indexed -> TrueColor
  }

  int n_pass = png_set_interlace_handling(png_ptr); // �C���^�[���X�摜�p

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
polymnia::IndexedPngLoader::load_(const char* path)
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

  if (type == PNG_RGB__)  // TrueColor�͑ʖ�
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return nullptr;
  }

  if (obits == 16)
  {
    png_set_strip_16(png_ptr); // 16bit�摜��8bit�ɒ���
    obits = 8;
  }
  if (alpha)
    png_set_strip_alpha(png_ptr); // Alpha�v�f�͖�������
  if (type == PNG_PAL__ && obits < 8)
    png_set_packing(png_ptr); // PAL8bit�ɓ���
  if (type == PNG_GRY__ && obits < 8)
    png_set_expand(png_ptr); // GRAY8bit�ɓ���

  int n_pass = png_set_interlace_handling(png_ptr); // �C���^�[���X�摜�p

  png_read_update_info(png_ptr, info_ptr);



  PictureIndexed* pct = PictureIndexed::create(ww, hh);


  // Palette�̃��[�h
  RgbColor* pal = pct->paletteBuffer();
  if (type == PNG_GRY__)
  {
    //�O���C�X�P�[���Ȃ玩���ŗp�ӂ���
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
