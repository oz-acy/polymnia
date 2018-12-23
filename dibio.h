/**************************************************************************
 *
 *  dibio.h
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  DIB IO
 *  DIB形式画像入出力用クラス
 *
 *  履歴
 *    2018.12.23 C++17對應 pathの渡し方を變更
 */

#ifndef INC_POLYMNIA_DIBIO_H___
#define INC_POLYMNIA_DIBIO_H___

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class DibLoader;
  class IndexedDibLoader;
  class DibSaver;
  class IndexedDibSaver;
}


/*-----------------------------------
 *  class DibLoader
 *  DIB 形式畫像のローダ
 *  24bit RGB 畫像として取り込む
 */
class polymnia::DibLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  DibLoader() {}
  ~DibLoader() {}
  polymnia::Picture* load(const std::filesystem::path& path) override;

//protected:
//  polymnia::Picture* load_(const char* path);
};


/*----------------------------------------------
 *  class IndexedDibLoader
 *  8bit 以下 indexed DIB 形式畫像のローダ
 *  8bit indexed の畫像データとして取り込む
 */
class polymnia::IndexedDibLoader
: public polymnia::PictLoader<polymnia::PictureIndexed>
{
public:
  IndexedDibLoader() {}
  ~IndexedDibLoader() {}
  polymnia::PictureIndexed* load(const std::filesystem::path& path) override;
//protected:
//  polymnia::PictureIndexed* load_(const char* path);
};


/*---------------------------------
 *  class DibSaver
 *  24bit DIB 形式畫像のセーバ
 */
class polymnia::DibSaver : public polymnia::PictSaver<polymnia::Picture>
{
public:
  DibSaver() {}
  ~DibSaver() {}
  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;

//protected:
//  bool save_(const polymnia::Picture* p, const char* path);
};


/*-----------------------------------------
 *  class IndexedDibSaver
 *  8bit indexed DIB 形式畫像のセーバ
 */
class polymnia::IndexedDibSaver
: public polymnia::PictSaver<polymnia::PictureIndexed>
{
public:
  IndexedDibSaver() {}
  ~IndexedDibSaver() {}

  bool
    save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;

//protected:
//  bool save_(const polymnia::PictureIndexed* p, const char* path);
};




#endif // INC_POLYMNIA_DIBIO_H___
