/*************************************************************************
 *
 *  tograyscale24.cpp
 *  by oZ/acy
 *  (c) 2007-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Picture の色變換ルーチン
 *
 *  履歴:
 *  2016.2.26 ファイル名變更
 *
 ************************************************************************/

#include "pictcvt.h"


/*================================================
*  convertToGrayScaled()
*  グレイスケール化
*/
void polymnia::convertToGrayScaled(polymnia::Picture* tgt) throw()
{
  using namespace themis;

  for (int y = 0; y < tgt->height(); y++) {
    for (int x = 0; x < tgt->width(); x++) {
      RgbColor col = tgt->pixel(x, y);
      UByte l = (UByte)(col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140);
      tgt->pixel(x, y) = RgbColor(l, l, l);
    }
  }
}




//eof
