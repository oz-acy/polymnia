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
 * @file jpegout.cpp
 * @author oZ/acy
 * @brief JPEG保存クラスの實裝
 *
 * @date 2018.12.23 修正
 *
 */

#include <cstdio>
#include "jpegio.h"

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

// 多重定義(オーバーロード)によつてfopen、_wfopenを切り替へる
inline
std::FILE* openfile(const char* path)
{
  using namespace std;
  return fopen(path, "wb");
}

inline
std::FILE* openfile(const wchar_t* path)
{
  using namespace std;
  return _wfopen(path, L"wb");
}

}//end of namespace NONAME




bool
polymnia::JpegSaver::save(
  const polymnia::Picture* pct, const std::filesystem::path& path)
{
  using namespace std;

  FILE *outfile = openfile(path.c_str());
  if (!outfile)
    return false;


  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  polymnia::private_::jpegErrorSetup_(jerr);

  try {
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = pct->width();
    cinfo.image_height = pct->height();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    if (prog)
      jpeg_simple_progression(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW buf[1];
    const polymnia::RgbColor* srcbuf = pct->buffer();
    int o = pct->offset();
    int j = 0;
    while(cinfo.next_scanline < cinfo.image_height) {
      buf[0] = (JSAMPROW)&srcbuf[j];
      jpeg_write_scanlines(&cinfo, buf, 1);
      j += o;
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);

    return true;
  }
  catch(themis::Exception& exp) {
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
    return false;
  }
}




//eof
