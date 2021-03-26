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
/**
 *  @file dibio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief DIB形式畫像入出力用クラス
 *
 *  @date 2018.12.23 C++17對應 pathの渡し方を變更
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.29 Loaderのload()の返却型をunique_ptrに變更
 */
#ifndef INCLUDE_GUARD_POLYMNIA_DIBIO_H
#define INCLUDE_GUARD_POLYMNIA_DIBIO_H

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
 *  RGB24bit色の畫像として讀み込む。
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
  std::unique_ptr<Picture> load(const std::filesystem::path& path) override;
};


/*----------------------------------------------*//**
 *  @brief 256パレットDIB形式畫像の讀み込み用クラス
 *
 *  DIB形式(indexed color)の畫像を256パレットの畫像として讀み込む。
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
  std::unique_ptr<PictureIndexed> 
  load(const std::filesystem::path& path) override;
};


/*---------------------------------------------------*//**
 *  @brief 24bit色DIB形式の畫像保存クラス
 *
 *  畫像を24ビット色DIB形式で保存する。
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
 *  @brief 256パレットDIB形式の畫像保存クラス
 *
 *  畫像を256パレットのDIB形式で保存する。
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
  /// 畫像を256パレットDIB形式で保存する。
  /// @param p 畫像
  /// @param path ファイルパス
  /// @return 成功したときはtrue、さもなくばfalse
  bool
    save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;
};




#endif // INCLUDE_GUARD_POLYMNIA_DIBIO_H
