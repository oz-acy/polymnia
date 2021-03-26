/*
 * Copyright 2001-2021 oZ/acy (名賀月晃嗣)
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
 *
 *  @file picture.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief 畫像バッファクラス二種の定義(24bit色、256パレット24bit色)
 * 
 *  @date 2016.3.2  C++11對應(假)
 *  @date 2018.12.28 グレイスケール化、擴大、縮小函數をメンバ函數化
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.29 createなどの返却型をunique_ptrに變更
 *  @date 2021.3.25 themis/inttype.hの非推奬化に對應
 */
#ifndef INCLUDE_GUARD_POLYMNIA_PICTURE_H
#define INCLUDE_GUARD_POLYMNIA_PICTURE_H

#include <string>
#include <cstring>
#include <memory>
#include "ibuf.h"
#include "rgb.h"


namespace polymnia
{
  class Picture;
  class PictureIndexed;
}


/**
 *  @brief RGB24bit色畫像バッファ
 */
class polymnia::Picture : public polymnia::ImageBuffer<polymnia::RgbColor>
{
protected:
  /// @brief 構築子
  /// @param w 幅
  /// @param h 高さ
  Picture(unsigned w, unsigned h)
    : polymnia::ImageBuffer<polymnia::RgbColor>(w, h, w)
    { buf_ = new polymnia::RgbColor[h_ * offset_]; }

public:
  /// @brief 解體子
  ~Picture() { delete[] buf_; }

  /// 畫像バッファを生成する。
  static std::unique_ptr<Picture> create(unsigned w, unsigned h) noexcept;

  /// 複製する。
  std::unique_ptr<Picture> clone() const noexcept;

  /// 256色に減色したPictureIndexedオブジェクトを生成する。
  std::unique_ptr<PictureIndexed> duplicatePictureIndexed() const noexcept;

  /// グレイスケール化する。
  void convertToGrayScaled() noexcept;

  /// 内容をグレイスケール化したPictureIndexedオブジェクトを生成する。
  std::unique_ptr<polymnia::PictureIndexed>
  createPictureGrayScaleIndexed() const noexcept;


  /// 縮小したPictureを生成する。
  std::unique_ptr<Picture> createReducedPicture(int w, int h) const noexcept;

  /// 擴大したPictureを生成する。
  std::unique_ptr<Picture> createMagnifiedPicture(int w, int h) const noexcept;
};




/**
 * @brief 256パレットRGB24bit色畫像バッファ
 */
class polymnia::PictureIndexed : public polymnia::ImageBuffer<std::uint8_t>
{
protected:
  polymnia::RgbColor pal_[256]; ///< パレット

  /// @brief 構築子
  /// @param w 幅
  /// @param h 高さ
  PictureIndexed(unsigned w, unsigned h)
    : polymnia::ImageBuffer<std::uint8_t>(w, h, w)
    { buf_ = new std::uint8_t[h_ * offset_]; }


public:
  /// 解體子
  ~PictureIndexed() { delete[] buf_; }

  /// 畫像バッファを生成する。
  static
  std::unique_ptr<PictureIndexed> create(unsigned w, unsigned h) noexcept;

  /// 複製する。
  std::unique_ptr<PictureIndexed> clone() const noexcept;


  // パレットへのアクセス
  /// パレットを參照する。
  /// @param id パレットID
  /// @return IDで指定されるパレットへの參照
  polymnia::RgbColor& palette(int id) noexcept { return pal_[id]; }
  /// パレットを參照する。
  /// @param id パレットID
  /// @return IDで指定されるパレットへの參照
  const polymnia::RgbColor& palette(int id) const noexcept { return pal_[id]; }
  /// パレットのバッファへのポインタを得る。
  polymnia::RgbColor* paletteBuffer() noexcept { return pal_; }
  /// パレットのバッファへのポインタを得る。
  const polymnia::RgbColor* paletteBuffer() const noexcept { return pal_; }

  /// 同内容のPictureを生成する。
  std::unique_ptr<Picture> duplicatePicture() const noexcept;
};



#endif // INCLUDE_GUARD_POLYMNIA_PICTURE_H
