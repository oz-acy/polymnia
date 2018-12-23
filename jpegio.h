/**************************************************************************
 *  jpegio.h
 *
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  JPEG IO
 *  JPEG形式画像入出力クラス (libjpegラッパ)
 *
 *  2018.12.23 C++17對應
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

  polymnia::Picture* load(const std::filesystem::path& path) override;
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

  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
};


#endif // INC_POLYMNIA_JPEGIO_H__
