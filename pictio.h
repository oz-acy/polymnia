/**************************************************************************
 *
 *  pictio.h
 *  by oZ/acy
 *  (c) 2012-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PICTue buffer IO
 *  畫像バッファのロード、セーブ用基底クラステンプレート
 *
 *  履歴
 *    2016.3.2   C++11對應
 *    2018.12.23 C++17對應 パスの渡し方を變更
 */

#ifndef INC_POLYMNIA_PICTURE_IO_H___
#define INC_POLYMNIA_PICTURE_IO_H___

#include <string>
#include <filesystem>
#include <boost/utility.hpp>


namespace polymnia
{
  template <class P> class PictLoader;
  template <class P> class PictSaver;
}


/*---------------------------------
 *  PictLoader<P_>
 *  畫像ロード用クラス基底
 */
template<class P_>
class polymnia::PictLoader : boost::noncopyable
{
public:
  PictLoader() noexcept {}
  virtual ~PictLoader() {}

  virtual P_* load(const std::filesystem::path& path) =0;
};


/*---------------------------------
 *  PictSaver<P_>
 *  畫像セーブ用クラス基底
 */
template<class P_>
class polymnia::PictSaver : boost::noncopyable
{
public:
  PictSaver() noexcept {}
  virtual ~PictSaver() {}

  virtual bool save(const P_*, const std::filesystem::path& path) =0;
};




#endif // INC_POLYMNIA_PICTURE_IO_H___
