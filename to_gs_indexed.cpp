/*************************************************************************
 *
 *  to_gs_indexed.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  グレイスケール化したPictureIndexedの生成
 *
 *  履歴
 *    2008.04.20 作成
 *    2008.10.03 修正
 *    2011.09.08 修正
 *    2016.02.26 ファイル名變更
 *    2016.03.02 C++11對應
 *    2018.12.28 createPictureGrayScaleIndexed()をPictureのメンバ函數に編入
 *    2019.8.29  返却型をunique_ptrに變更
 */
#include "picture.h"


/*================================================
 *  グレイスケール + Indexed化
 */
std::unique_ptr<polymnia::PictureIndexed>
polymnia::Picture::createPictureGrayScaleIndexed() const noexcept
{
  using namespace themis;

  /* PictureIndexed生成 */
  auto pc = PictureIndexed::create(w_, h_);
  //PictureIndexed* pc = PictureIndexed::create(w_, h_);
  if (!pc)
    return nullptr;

  /* パレット作成 */
  for (int i = 0; i < 256; i++)
    pc->palette(i) = RgbColor((UByte)i, (UByte)i, (UByte)i);

  /* グレイスケール化 */

  for (int y = 0; y < h_; y++) {
    for (int x = 0; x < w_; x++) {
      RgbColor col = pixel(x, y);
      UByte l = col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140;
      pc->pixel(x, y) = l;
    }
  }

  return pc;
}




//eof
