/*
 * Copyright 2002-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * @file jpegin.cpp
 * @file oZ/acy (名賀月晃嗣)
 * @brief JPEG讀み込みクラスの實裝
 *
 */
#include<cstdio>
#include"jpegio.h"

extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}

#include <themis/exception.h>


namespace polymnia
{
  namespace private_
  {
    void jpegErrorSetup_(jpeg_error_mgr& jerr);
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
//
std::unique_ptr<polymnia::Picture>
polymnia::JpegLoader::load(const std::filesystem::path& path)
{
  FILE *infile;
  infile = openfile(path.c_str());
  if (!infile)
    return nullptr;

  // エラールーチンセットアップ
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  polymnia::private_::jpegErrorSetup_(jerr);

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

    auto q = Picture::create(ww,hh);
    if (!q)
      throw themis::Exception("JPEGLoader", "load()", "object creating fault.");

    JSAMPROW buf[1];
    RgbColor* resbuf = q->buffer();
    int o = q->offset();
    int j = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
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
