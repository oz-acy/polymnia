/*************************************************************************
 *
 *  tograyscale24.cpp
 *  by oZ/acy
 *  (c) 2007-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Picture の色變換ルーチン
 *
 *  履歴:
 *  2016.2.26 ファイル名變更
 *  2016.3.2  C++對應
 *  2018.12.28 convertToGrayScaledをPictureのメンバ函數に編入
 *
 */

#include "picture.h"


/*================================================
 *  convertToGrayScaled()
 *  グレイスケール化
 */
void polymnia::Picture::convertToGrayScaled() noexcept
{
  for (int i = 0; i < h_ * offset_; i++) {
    themis::UByte l
      = buf_[i].r * 0.2990 + buf_[i].g * 0.5870 + buf_[i].b * 0.1140;
    buf_[i] = RgbColor(l, l, l);
  }

  /*
  using namespace themis;

  for (int y = 0; y < tgt->height(); y++) {
    for (int x = 0; x < tgt->width(); x++) {
      RgbColor col = tgt->pixel(x, y);
      UByte l = (UByte)(col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140);
      tgt->pixel(x, y) = RgbColor(l, l, l);
    }
  }
  */
}




//eof
