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
 *  @file rgb.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief RGB24bit色情報クラス
 *
 *  @date 2016.3.2  C++11/14對應(假)
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2021.3.25 <stdint>に定義された整数型を直接參照するやうに修正
 */
#ifndef INCLUDE_GUARD_POLYMNIA_RGBCOLOUR_H
#define INCLUDE_GUARD_POLYMNIA_RGBCOLOUR_H

#include <cstdint>

namespace polymnia
{
/**
 * @brief RGB24bit色情報クラス
 */
class RgbColor
{
public:
  std::uint8_t r; ///< 赤要素
  std::uint8_t g; ///< 緑要素
  std::uint8_t b; ///< 青要素

public:
  /// @brief 構築子
  ///
  /// RGB(0, 0, 0)で初期化する。
  constexpr RgbColor() noexcept : r(0), g(0), b(0) {}

  /// @brief 構築子
  ///
  /// RGB値を指定して初期化する。
  constexpr 
  RgbColor(std::uint8_t r0, std::uint8_t g0, std::uint8_t b0) noexcept
    : r(r0), g(g0), b(b0)
    {}

  /// @brief 等價比較演算子
  constexpr
  bool operator==(const RgbColor& other) const noexcept
  {
    return r == other.r && g == other.g && b == other.b;
  }

  /// @brief 非等價比較演算子
  constexpr
  bool operator!=(const RgbColor& other) const noexcept
  {
    return !(*this == other);
  }
};


}//end of namespace polymnia


#endif // INCLUDE_GUARD_POLYMNIA_RGBCOLOUR_H
