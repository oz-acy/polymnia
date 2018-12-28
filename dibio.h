/**********************************************************************//**
 *
 *  @file dibio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief DIB形式畫像入出力用クラス
 *
 *  @date 2018.12.23 C++17對應 pathの渡し方を變更
 *//*
 *  (c) 2002-2018 oZ/acy. ALL RIGHTS RESERVED.
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


/*------------------------------------------*//**
 *  @brief DIB形式畫像の讀み込み用クラス
 *
 *  DIB形式の畫像を
 *  RGB24bitカラーの畫像として讀み込む。
 */
class polymnia::DibLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  /// @brief 構築子
  DibLoader() {}
  /// @brief 解體子
  ~DibLoader() {}

  /// @brief 讀み込み
  ///
  /// 指定されたファイルから畫像を讀み込む。
  /// @param path ファイルパス
  /// @return
  ///  讀み込んだ畫像を格納した畫像バッファオブジェクトを返す。
  ///  ファイル形式が異なるなど讀み込めない場合にはnullptrを返す。
  polymnia::Picture* load(const std::filesystem::path& path) override;
};


/*----------------------------------------------*//**
 *  @brief DIB形式畫像の讀み込みクラス
 *
 *  DIB形式(indexed color)の畫像を
 *  256パレットカラーの畫像として讀み込む。
 */
class polymnia::IndexedDibLoader
: public polymnia::PictLoader<polymnia::PictureIndexed>
{
public:
  /// @brief 構築子
  IndexedDibLoader() {}
  /// @brief 解體子
  ~IndexedDibLoader() {}

  /// @brief 讀み込み
  ///
  /// 指定されたファイルから畫像を讀み込む。
  /// @param path ファイルパス
  /// @return
  ///  讀み込んだ畫像を格納した畫像バッファオブジェクトを返す。
  ///  ファイル形式が異なるなど讀み込めない場合にはnullptrを返す。
  polymnia::PictureIndexed* load(const std::filesystem::path& path) override;
};


/*---------------------------------------------------*//**
 *  @brief 24bitカラーDIB形式の畫像保存クラス
 */
class polymnia::DibSaver : public polymnia::PictSaver<polymnia::Picture>
{
public:
  /// @brief 構築子
  DibSaver() {}
  /// @brief 解體子
  ~DibSaver() {}

  /// @brief 保存
  ///
  /// 畫像を24bitカラーDIB形式で保存する。
  /// @param p 畫像
  /// @param path ファイルパス
  /// @return 成功したときはtrue、さもなくばfalse
  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
};


/*------------------------------------------------------*//**
 *  @brief 256パレットカラーDIB形式の畫像保存クラス
 */
class polymnia::IndexedDibSaver
: public polymnia::PictSaver<polymnia::PictureIndexed>
{
public:
  /// @brief 構築子
  IndexedDibSaver() {}
  /// @brief 解體子
  ~IndexedDibSaver() {}

  /// @brief 保存
  ///
  /// 畫像を256パレットカラーDIB形式で保存する。
  /// @param p 畫像
  /// @param path ファイルパス
  /// @return 成功したときはtrue、さもなくばfalse
  bool
    save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;
};




#endif // INC_POLYMNIA_DIBIO_H___
