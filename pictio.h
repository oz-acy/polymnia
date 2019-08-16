/**********************************************************************//**
 *
 *  @file pictio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief 畫像讀み込み及び保存用基底クラステンプレート
 *
 *  @date 2016.3.2   C++11對應
 *  @date 2018.12.23 C++17對應 パスの渡し方を變更
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 */
#ifndef INCLUDE_GUARD_POLYMNIA_PICTURE_IO_H
#define INCLUDE_GUARD_POLYMNIA_PICTURE_IO_H

#include <string>
#include <filesystem>
#include <boost/utility.hpp>


namespace polymnia
{
  template <class P> class PictLoader;
  template <class P> class PictSaver;
}


/*----------------------------------------------------*//**
 *  @brief 畫像讀み込み用基底クラステンプレート
 */
template<class P_>
class polymnia::PictLoader : boost::noncopyable
{
public:
  /// @brief 構築子
  PictLoader() noexcept {}
  /// @brief 解體子
  virtual ~PictLoader() {}

  /// @brief 讀み込み
  ///
  /// 讀み込み處理は派生クラスで實裝する。
  virtual P_* load(const std::filesystem::path& path) =0;
};


/*--------------------------------------------------*//**
 *  @brief 畫像保存用基底クラステンプレート
 */
template<class P_>
class polymnia::PictSaver : boost::noncopyable
{
public:
  /// @brief 構築子
  PictSaver() noexcept {}
  /// @brief 解體子
  virtual ~PictSaver() {}

  /// @brief 保存
  ///
  /// 保存處理は派生クラスで實裝する。
  virtual bool save(const P_*, const std::filesystem::path& path) =0;
};




#endif // INCLUDE_GUARD_POLYMNIA_PICTURE_IO_H
