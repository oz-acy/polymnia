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
 *  @file jpegio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief JPEG畫像入出力クラス (libjpegのラッパー)
 *
 *  @date 2018.12.23 C++17對應
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.29 JpegLoader::loadの返却型をunique_ptrに變更
 */
#ifndef INCLUDE_GUARD_POLYMNIA_JPEGIO_H
#define INCLUDE_GUARD_POLYMNIA_JPEGIO_H

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class JpegLoader;
  class JpegSaver;
}


/*--------------------------------------------*//**
 *  @brief JPEG畫像讀み込みクラス
 */
class polymnia::JpegLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  /// @brief 構築子
  JpegLoader() {}
  /// @brief 解體子
  ~JpegLoader() {}

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


/*------------------------------------------*//**
 *  @brief JPEG畫像保存クラス
 */
class polymnia::JpegSaver : public polymnia::PictSaver<polymnia::Picture>
{
private:
  int quality;  ///< クオリティ
  bool prog;    ///< プログレッシブ形式で保存するか否か

public:
  /// @brief 構築子
  /// @param q クオリティ。0〜100のいづれかを指定する。デフォルト値は75。
  /// @param g 
  ///   プログレッシブ形式で保存する場合はtrue、さもなくばfalse。
  ///   デフォルト値はfalse。
  JpegSaver(int q = 75, bool g = false) : quality(q), prog(g) {}

  /// @brief 解體子
  ~JpegSaver() {}

  /// @brief クオリティの設定
  ///
  /// クオリティを0〜100の間で設定する。
  void setQuality(int q) { quality = q; }

  /// @brief プログレッシブ形式で保存するか否かの設定
  ///
  /// プログレッシブ形式で保存する場合はtrue、
  /// さもなくばfalseを設定する。
  void setProgressive(bool g) { prog = g; }

  /// @brief 保存
  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
};


#endif // INCLUDE_GUARD_POLYMNIA_JPEGIO_H
