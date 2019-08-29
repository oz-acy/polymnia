/**********************************************************************//**
 *
 *  @file pictio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief 畫像讀み込み及び保存用基底クラステンプレート
 *
 *  @date 2016.3.2   C++11對應
 *  @date 2018.12.23 C++17對應 パスの渡し方を變更
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.26 boost::noncopyableをthemis::Noncopyable<>に差し替へ
 *  @date 2019.8.29 PictLoader::load()の返却型をunique_ptrに變更
 */
#ifndef INCLUDE_GUARD_POLYMNIA_PICTURE_IO_H
#define INCLUDE_GUARD_POLYMNIA_PICTURE_IO_H

#include <string>
#include <filesystem>
#include <memory>
#include <themis/noncopyable.h>


namespace polymnia
{
  template <class P> class PictLoader;
  template <class P> class PictSaver;
}


/*----------------------------------------------------*//**
 *  畫像讀み込み用基底クラステンプレート
 */
template<class P_>
class polymnia::PictLoader : themis::Noncopyable<polymnia::PictLoader<P_>>
{
public:
  /// 構築子
  PictLoader() noexcept {}
  /// 解體子
  virtual ~PictLoader() {}

  /// @brief 讀み込み
  ///
  /// 讀み込み處理は派生クラスで實裝する。
  //virtual P_* load(const std::filesystem::path& path) =0;
  virtual std::unique_ptr<P_> load(const std::filesystem::path& path) =0;
};


/*--------------------------------------------------*//**
 *  @brief 畫像保存用基底クラステンプレート
 */
template<class P_>
class polymnia::PictSaver : themis::Noncopyable<polymnia::PictSaver<P_>>
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
