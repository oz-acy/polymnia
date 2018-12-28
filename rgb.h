/**********************************************************************//**
 *
 *  @file rgb.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief RGB24bitカラー型
 *
 *  @date 2016.3.2  C++11/14對應(假)
 *//*
 *  (c) 2001-2016 oZ/acy. ALL RIGHTS RESERVED.
 */
#ifndef INC_POLYMNIA_RGBCOL_H___
#define INC_POLYMNIA_RGBCOL_H___

#include <themis/inttype.h>

namespace polymnia
{
/**
 * @brief RGB24bit色情報クラス
 */
class RgbColor
{
public:
  themis::Byte r;  ///< 赤要素
  themis::Byte g;  ///< 緑要素
  themis::Byte b;  ///< 青要素

public:
  /// @brief 構築子
  ///
  /// RGB(0, 0, 0)で初期化する。
  constexpr RgbColor() noexcept : r(0), g(0), b(0) {}

  /// @brief 構築子
  ///
  /// RGB値を指定して初期化する。
  constexpr 
  RgbColor(themis::Byte r0, themis::Byte g0, themis::Byte b0) noexcept
    : r(r0), g(g0), b(b0)
    {}

  /// @brief 等値比較演算子
  bool operator==(const RgbColor& other) const noexcept
  {
    return r == other.r && g == other.g && b == other.b;
  }

  /// @brief 非等値比較演算子
  bool operator!=(const RgbColor& other) const noexcept
  {
    return !(*this == other);
  }
};


}//end of namespace polymnia


#endif // INC_POLYMNIA_RGBCOL_H___
