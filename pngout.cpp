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
 * @file pngout.cpp
 * @author oZ/acy
 * @brief PNG形式出力クラスの實裝
 *
 * @date 2019.8.27 修正
 * @date 2021.3.26 修正
 *
 */
#include <cstdio>
#include <iostream>
#include <fstream>

extern "C" {
#include <png.h>
}

#include "pngio.h"


namespace
{

/*________________________________________
 *
 *  ofstream に對する書き込み
 *
 *  H23.4.24 update for libpng-1.5.x
 */
void pngWrite_(png_structp png_ptr, png_bytep data, png_size_t length)
{
  using namespace std;
  ofstream* pofs = reinterpret_cast<ofstream*>(png_get_io_ptr(png_ptr));
  pofs->write(reinterpret_cast<char*>(data), length);

  //  H23.4.24 comment out for libpng-1.5.x
  //((std::ofstream*)(png_ptr->io_ptr))->write((char*)data,length);
}


/* flush操作 (但しダミー) */
void pngFlushDammy_(png_structp)
{
  /* none */
}


/* 書き込みの初期處理 */
bool pngWriteInit_(
  const std::filesystem::path& path, std::ofstream& ofs, png_structp& png_ptr,
  png_infop& info_ptr)
{
  ofs.open(path, std::ios::out|std::ios::binary);
  if (!ofs)
    return false;

  png_ptr
    = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr)
    return false;

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    png_destroy_write_struct(&png_ptr, nullptr);
    return false;
  }

  png_set_write_fn(png_ptr, (void *)&ofs, pngWrite_, pngFlushDammy_);

  return true;
}


}//end of namespace NONAME


bool
polymnia::PngSaver::save(
  const polymnia::Picture* pct, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngWriteInit_(path, ofs, png_ptr, info_ptr))
    return false;

  int bits = 8;
  int type = PNG_COLOR_TYPE_RGB;
  int i_val = interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
  int ww = pct->width();
  int hh = pct->height();

  png_set_IHDR(
    png_ptr, info_ptr, ww, hh, bits, type, i_val, PNG_COMPRESSION_TYPE_BASE,
    PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  // インターレス化する場合の繰り返し囘數を取得 (しない場合は1固定)
  int n_pass;
  if (interlace)
    n_pass = png_set_interlace_handling(png_ptr);
  else
    n_pass = 1;


  const RgbColor* buf = pct->buffer();
  int o = pct->offset();
  for (int pass=0; pass < n_pass; pass++)
    for (int j=0, y=0; y < hh; y++, j+=o)
    {
      png_bytep pp = (png_bytep)&buf[j];
      png_write_rows(png_ptr, &pp, 1);
    }

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  return true;
}


bool
polymnia::IndexedPngSaver::save(
  const polymnia::PictureIndexed* pct, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngWriteInit_(path, ofs, png_ptr, info_ptr))
    return false;

  int bits=8;
  int type = PNG_COLOR_TYPE_PALETTE;
  int i_val = interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
  int ww = pct->width();
  int hh = pct->height();
  png_set_IHDR(
    png_ptr, info_ptr, ww, hh, bits, type, i_val, PNG_COMPRESSION_TYPE_BASE,
    PNG_FILTER_TYPE_BASE);


  // パレット関係
  auto palette = std::make_unique<png_color[]>(256);
  //png_colorp palette = new png_color[256];
  for (int i=0; i<256; i++) {
    palette[i].red = pct->palette(i).r;
    palette[i].green = pct->palette(i).g;
    palette[i].blue = pct->palette(i).b;
  }
  png_set_PLTE(png_ptr, info_ptr, palette.get(), 256);

  // 透過処理
  auto transbf = std::make_unique<png_byte[]>(256);
  //png_bytep transbf = new png_byte[256];
  if (trans) {
    for (int i=0; i < 256; i++)
      if (i == paltp)
        transbf[i] = 0;
      else
        transbf[i] = 0xff;

    png_set_tRNS(png_ptr, info_ptr, transbf.get(), 256, nullptr);
  }

  png_write_info(png_ptr,info_ptr);


  // インターレス化する場合の繰り返し囘數を取得 (しない場合は1固定)
  int n_pass;
  if(interlace)
    n_pass = png_set_interlace_handling(png_ptr);
  else
    n_pass = 1;


  const std::uint8_t* buf = pct->buffer();
  int o = pct->offset();
  for (int pass = 0; pass < n_pass; pass++)
    for (int j=0, y=0; y < hh; y++, j+=o) {
      png_bytep pp = (png_bytep)&buf[j];
      png_write_rows(png_ptr,&pp,1);
    }

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  //delete[] palette;
  //delete[] transbf;
  return true;
}


//eof
