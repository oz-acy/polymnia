/**************************************************************************
 *  jpegio.cpp
 *
 *  (C) 2002-2013 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  JPEG形式入出力 エラーハンドラ
 *
 *  last update: 2013.3.29
 *
 ************************************************************************/

#include <themis/except.h>

extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}


namespace
{

void myErrorOutput__(j_common_ptr)
{
  /* nothing done */
}

void myErrorExit__(j_common_ptr cinfo)
{
  char buf[JMSG_LENGTH_MAX];

  (*cinfo->err->output_message)(cinfo);
  (*cinfo->err->format_message)(cinfo,buf);
  throw themis::Exception("jpeg-I/O", "error", buf);
}


}//end of namespace


namespace polymnia
{
namespace private___
{

void jpegErrorSetup__(jpeg_error_mgr& jerr)
{
  jerr.error_exit = myErrorExit__;
  jerr.output_message = myErrorOutput__;
}


}//end of namespace private___
}//end of namespace polymnia


//eof
