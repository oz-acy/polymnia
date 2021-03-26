/*
 * Copyright 2003-2021 oZ/acy (名賀月晃嗣)
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
/*
 * @file reduce.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief Picture用縮小ルーチン
 *
 * @date 27 Nov MMIV    resize.cpp から縮小專用ルーチンを切り分け
 * @date 27 Feb MMVIII  LIBPOLYMNIAに移動
 * @date 20 Apr MMVIII  PictureをLIBPOLYMNIAに移動したことによる修正
 * @date  8 Sep MMXI    NULL を nullptr に修正(C++11)
 * @date 28 Jul MMXII   バグ修正(メモリ不正アクセス)
 * @date  2 Mar MMXVI   throw()を削除
 * @date 28 Dec MMXVIII createReducedPicture()をPictureのメンバ函數に編入
 * @date 27 Aug MMXIX   __を含む名前を修正
 * @date 29 Aug MMXIX   createReducedPicture()の返却型をunique_ptrに變更
 * @date 25 Mar MMXXI   themis/inttype.hの非推奬化に對應
 *
 */
#include "picture.h"


namespace {

polymnia::RgbColor
toSmall_(
  const polymnia::Picture* orig, double x1, double y1, double x2, double y2)
{
  using namespace polymnia;
  using namespace themis;

  int sx = (int)x1;
  int sy = (int)y1;
  int ex = (int)x2;
  if ((double)ex != x2)
    ex++;
  int ey = (int)y2;
  if ((double)ey != y2)
    ey++;

  double S = 0.0;  // 總面積
  double R = 0.0;
  double G = 0.0;
  double B = 0.0;

  for (int y = sy; y < ey; y++) {
    double e1 = (double)y;
    double e2 = (double)(y + 1);
    if (e1 < y1)
      e1 = y1;  // 上端
    if (e2 > y2)
      e2 = y2;  // 下端

    double ph = e2 - e1;  // 高さ

    for (int x = sx; x < ex; x++) {
      double e3 = (double)x;
      double e4 = (double)(x + 1);
      if (e3 < x1)
        e3 = x1;  // 左端
      if (e4 > x2)
        e4 = x2;  // 右端

      double pw = e4 - e3;  // 幅

      double ss = ph * pw;  // 面積
      S += ss;

      R += (double)(orig->pixel(x, y).r) * ss;
      G += (double)(orig->pixel(x, y).g) * ss;
      B += (double)(orig->pixel(x, y).b) * ss;
    }
  }
  if (S == 0.0)
    return RgbColor(0, 0, 0);

  R /= S;
  G /= S;
  B /= S;

  return
    RgbColor(
      (std::uint8_t)(R + 0.5),
      (std::uint8_t)(G + 0.5),
      (std::uint8_t)(B + 0.5));
}


}//end of namespace


/*================================================================
 *  createReducedPicture()
 *  畫像縮小ルーチン
 *==============================================================*/
std::unique_ptr<polymnia::Picture>
polymnia::Picture::createReducedPicture(int w, int h) const noexcept
{
  using namespace polymnia;

  auto pict = Picture::create(w, h);
  if (!pict)
    return nullptr;

  double dw = (double)width() / (double)w;
  double dh = (double)height() / (double)h;

  for (int Y = 0; Y < h; Y++)
  {
    double y1 = (double)Y * dh;
    double y2 = (double)(Y + 1) * dh;
    
    if (y2 > height())
      break;

    for (int X = 0; X < w; X++)
    {
      double x1 = (double)X * dw;
      double x2 = (double)(X + 1) * dw;

      if (x2 > width())
        break;

      pict->pixel(X, Y) = toSmall_(this, x1, y1, x2, y2);
    }
  }

  return pict;
}




//eof
