/**************************************************************************
*
*  jpegin.cpp
*  by oZ/acy
*  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
*
*  JPEG 形式入力クラス
*
*  last update: 2018.12.23
*
**************************************************************************/

#include<cstdio>
#include"jpegio.h"

extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}

#include <themis/except.h>


namespace polymnia
{
  namespace private___
  {
    void jpegErrorSetup__(jpeg_error_mgr& jerr);
  }
}


namespace {

// オーバーロードによつて fopen、_wfopenを切り替へる
inline
std::FILE* openfile(const char* path)
{
  using namespace std;
  return fopen(path, "rb");
}

inline
std::FILE* openfile(const wchar_t* path)
{
  using namespace std;
  return _wfopen(path, L"rb");
}


}//end of namespace NONAME



////////////////////////////////////////
// JpegLoader::load_()の本體
polymnia::Picture* polymnia::JpegLoader::load(const std::filesystem::path& path)
{
  FILE *infile;
  infile = openfile(path.c_str());
  if (!infile)
    return nullptr;

  // エラールーチンセットアップ
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  polymnia::private___::jpegErrorSetup__(jerr);

  try
  {
    jpeg_create_decompress(&cinfo); // cinfo情報のアロケートと初期化
    jpeg_stdio_src(&cinfo, infile); // データソースの指定
    jpeg_read_header(&cinfo, TRUE); // JPEGファイルのパラメータ情報の読み込み
    jpeg_start_decompress(&cinfo);  // 解凍開始

    int ww = cinfo.output_width;
    int hh = cinfo.output_height;
    int bit = cinfo.output_components;
    if (bit != 3)
      throw themis::Exception("JPEGLoader","load()","bits!=3");

    Picture* q = Picture::create(ww,hh);
    if (!q)
      throw themis::Exception("JPEGLoader", "load()", "object creating fault.");

    JSAMPROW buf[1];
    RgbColor* resbuf = q->buffer();
    int o = q->offset();
    int j = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
      buf[0] = (JSAMPROW)&resbuf[j];
      jpeg_read_scanlines(&cinfo, buf, 1);
      j += o;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return q;
  }
  catch(themis::Exception& exp)
  {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return nullptr;
  }
}


//eof
