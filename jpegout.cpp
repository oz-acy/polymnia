/**************************************************************************
 *
 *  jpegout.cpp
 *  by oZ/acy
 *  (c) 2002-2013 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  libjpegÇ…ÇÊÇÈJPEGèoóÕÉãÅ[É`Éì
 *
 *  last update: 2013.3.29
 *
 *************************************************************************/

#include <cstdio>
#include "jpegio.h"

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


bool
polymnia::JpegSaver::save_(const polymnia::Picture* pct, const char* path)
{
  using namespace std;

  FILE *outfile = fopen(path, "wb");
  if (!outfile)
    return false;

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  private___::jpegErrorSetup__(jerr);

  try
  {
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = pct->width();
    cinfo.image_height = pct->height();
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,quality,TRUE);
    if (prog)
      jpeg_simple_progression(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW buf[1];
    const RgbColor* srcbuf = pct->buffer();
    int o = pct->offset();
    int j=0;
    while(cinfo.next_scanline<cinfo.image_height)
    {
      buf[0] = (JSAMPROW)&srcbuf[j];
      jpeg_write_scanlines(&cinfo,buf,1);
      j+=o;
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);

    return true;
  }
  catch(themis::Exception exp)
  {
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
    return false;
  }
}


//eof
