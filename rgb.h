/**************************************************************************
 *
 *  rgb.h
 *  by oZ/acy
 *  (c) 2001-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  RGB (24bit) color type
 *  各8bit RGB Color 型
 *
 *  履歴
 *    2016.2.27  C++11/14對應(假)
 *
 *************************************************************************/

#ifndef INC_POLYMNIA_RGBCOL_H___
#define INC_POLYMNIA_RGBCOL_H___

#include <themis/inttype.h>

namespace polymnia
{
/*--------------------------
*  class RgbColor
*  RGB24bit色情報クラス
*------------------------*/
class RgbColor
{
public:
  themis::Byte r, g, b;

public:
  constexpr RgbColor() throw() : r(0), g(0), b(0) {}
  constexpr RgbColor(themis::Byte r0, themis::Byte g0, themis::Byte b0) throw()
    : r(r0), g(g0), b(b0)
    {}

  bool operator==(const RgbColor& other) const throw()
  {
    return r == other.r && g == other.g && b == other.b;
  }

  bool operator!=(const RgbColor& other) const throw()
  {
    return !(*this == other);
  }
};


}//end of namespace polymnia


#endif // INC_POLYMNIA_RGBCOL_H___
