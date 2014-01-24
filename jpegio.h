/**************************************************************************
 *  jpegio.h
 *
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  JPEG IO
 *  JPEG形式画像入出力クラス (libjpegラッパ)
 *
 *  last update: 2012.3.1
 *
 */

#ifndef INC_POLYMNIA_JPEGIO_H__
#define INC_POLYMNIA_JPEGIO_H__

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class JpegLoader;
  class JpegSaver;
}


/*---------------------
 *  JpegLoader
 *  JPEG形式ローダ
 *-------------------*/
class polymnia::JpegLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  JpegLoader() {}
  ~JpegLoader() {}

protected:
  polymnia::Picture* load_(const char* path);
};


/*---------------------
 *  JpegSaver
 *  JPEG形式セーバ
 *-------------------*/
class polymnia::JpegSaver : public polymnia::PictSaver<polymnia::Picture>
{
private:
  int quality;
  bool prog;

public:
  JpegSaver(int q=75, bool g =false) : quality(q), prog(g) {}
  ~JpegSaver() {}
  void setQuality(int q) { quality = q; }
  void setProgressive(bool g) { prog = g; }

protected:
  bool save_(const polymnia::Picture* p, const char* path);
};


#endif // INC_POLYMNIA_JPEGIO_H__
