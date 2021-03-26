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
 *  @file pngoio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief PNG形式畫像入出力クラス (libpngのラッパー)
 *
 *  @date 2018.12.23 C++17對應
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.29 Loaderのload()の返却型をunique_ptrに變更
 *  @date 2021.3.26 themis/inttype.hの非推奬化に對應
 *
 */
/*
 *  Classes defined in this file are inplemented with "libpng" library.
 *  The author thanks authors of "libpng."
 */
#ifndef INCLUDE_GUARD_POLYMNIA_PNGIO_H
#define INCLUDE_GUARD_POLYMNIA_PNGIO_H

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class PngLoader;
  class IndexedPngLoader;
  class PngSaver;
  class IndexedPngSaver;
}


/*--------------------------------------------*//**
 *  @brief PNG形式畫像の讀み込みクラス
 *
 *  RGB24bitカラーの畫像として讀み込む。
 */
class polymnia::PngLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  /// @brief 構築子
  PngLoader() {}
  /// @brief 解體子
  ~PngLoader() {}

  /// @brief 讀み込み
  ///
  /// 指定されたファイルから畫像を讀み込む。
  /// @param path ファイルパス
  /// @return
  ///  讀み込んだ畫像を格納した畫像バッファオブジェクトを返す。
  ///  ファイル形式が異なるなど讀み込めない場合にはnullptrを返す。
  std::unique_ptr<polymnia::Picture>
  load(const std::filesystem::path& path) override;
};


/*-----------------------------------------------*//**
 *  @brief PNG形式畫像の讀み込みクラス
 *
 *  256パレットカラーの畫像として讀み込む。
 */
class polymnia::IndexedPngLoader
: public polymnia::PictLoader<polymnia::PictureIndexed>
{
public:
  /// @brief 構築子
  IndexedPngLoader() {}
  /// @brief 解體子
  ~IndexedPngLoader() {}

  /// @brief 讀み込み
  ///
  /// 指定されたファイルから畫像を讀み込む。
  /// @param path ファイルパス
  /// @return
  ///  讀み込んだ畫像を格納した畫像バッファオブジェクトを返す。
  ///  ファイル形式が異なるなど讀み込めない場合にはnullptrを返す。
  std::unique_ptr<polymnia::PictureIndexed>
  load(const std::filesystem::path& path) override;
};



/*-----------------------------------------------*//**
 *  @brief 24bitカラーPNG形式の畫像保存クラス
 */
class polymnia::PngSaver : public polymnia::PictSaver<polymnia::Picture>
{
private:
  bool interlace; ///< インターレース化するか否か

public:
  /// @brief 構築子
  ///
  /// デフォルトではインターレース化しないやうに設定する。
  PngSaver() : interlace(false) {}
  /// @brief 解體子
  ~PngSaver() {}

  /// @brief インターレース化するやうに設定
  void enableInterlace() { interlace = true; }
  /// @brief インターレース化しないやうに設定
  void disableInterlace() { interlace = false; }

  /// @brief 保存
  bool
  save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
};


/*-------------------------------------------------------*//**
 *  @brief 256パレットカラーPNG形式の畫像保存クラス
 */
class polymnia::IndexedPngSaver
: public polymnia::PictSaver<polymnia::PictureIndexed>
{
private:
  bool interlace;      ///< インターレース化するか否か
  bool trans;          ///< 透過色を設定するか否か
  std::uint8_t paltp;  ///< 透過色

public:
  /// @brief 構築子
  ///
  /// デフォルトではインターレース化せず、透過色も持たぬやう設定する。
  IndexedPngSaver() : interlace(false), trans(false), paltp(0) {}
  /// @brief 解體子
  ~IndexedPngSaver() {}

  /// @brief インターレース化するやうに設定
  void enableInterlace() { interlace = true; }
  /// @brief インターレース化しないやうに設定
  void disableInterlace() { interlace = false; }
  
  /// @brief 透過色を設定
  void enableTransparent(std::uint8_t pl)
  {
    trans = true;
    paltp = pl;
  }
  /// @brief 透過色を持たぬやう設定
  void disableTransparent() { trans = false; }

  /// @brief 保存
  bool
  save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;
};




#endif // INCLUDE_GUARD_POLYMNIA_PNGIO_H
