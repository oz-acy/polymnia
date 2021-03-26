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
