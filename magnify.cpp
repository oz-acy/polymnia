/**************************************************************************
 *
 *  magnify.cpp
 *
 *  (C) 2003-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Picture用擴大縮小ルーチン
 *
 *  履歴
 *    27 Nov MMIV    擴大ルーチンのアルゴリズムを變更
 *    27 Fev MMVIII  LIBPOLYMNIAに移行
 *    20 Apl MMVIII  LIBPOLYMNIAの構成變更に伴ひ修正
 *     8 Sep MMXI    NULL を nullptr に修正 (C++11)
 *    26 Feb MMXVI   ファイル名變更
 *     2 Mar MMXVI   throw()を削除
 *    28 Dec MMXVIII createMagnifiedPictureをPictureのメンバ函數に編入
 */
#include "picture.h"

namespace {

double fCubic_(double t)
{
  if (t < 0)
    t = -t;

  if (t < 1.0)
    return t * t * (t - 2.0) + 1.0;
  else if (t < 2.0)
    return -t * (t * (t - 5.0) + 8.0) + 4.0;
  else
    return 0.0;
}

double innerProduct_(int n, double* x, double* y)
{
  double res = 0.0;
  for (int i=0; i < n; i++)
    res += x[i] * y[i];
  return res;
}

void productMat14_44_(double* res, double* vec, double* mat)
{
  for (int i=0; i < 4; i++)
  {
    res[i] = 0.0;
    for (int j=0; j < 4; j++)
      res[i] += vec[j] * mat[i + j * 4];
  }
}


}// end of namespace




/*================================================
 *  createMagnifiedPicture()
 *  擴大畫像生成ルーチン
 *==============================================*/
polymnia::Picture* 
polymnia::Picture::createMagnifiedPicture(int w, int h) const noexcept
{
  using namespace polymnia;
  using namespace themis;

  Picture* pict = Picture::create(w, h);
  if (!pict)
    return nullptr;

  double nrx = width() / (double)w;
  double nry = height() / (double)h;

  for (int Y = 0; Y < h; Y++)
  {
    double y0 = Y * nry;
    double dy = y0 - (int)y0;

    double fy[4];
    fy[0] = fCubic_(1.0 + dy);
    fy[1] = fCubic_(dy);
    fy[2] = fCubic_(1.0 - dy);
    fy[3] = fCubic_(2.0 - dy);

    int py[4];
    for (int i=0; i < 4; i++)
    {
      py[i] = (int)y0 - 1 + i;
      if (py[i] < 0)
        py[i] = 0;
      else if (py[i] >= height())
        py[i] = height() - 1;
    }

    for (int X = 0; X < w; X++)
    {
      double x0 = X * nrx;
      double dx = x0 - (int)x0;

      double fx[4];
      fx[0] = fCubic_(1.0 + dx);
      fx[1] = fCubic_(dx);
      fx[2] = fCubic_(1.0 - dx);
      fx[3] = fCubic_(2.0 - dx);

      int px[4];
      for (int i=0; i < 4; i++)
      {
        px[i] = (int)x0 - 1 + i;
        if (px[i] < 0)
          px[i] = 0;
        else if (px[i] >= width())
          px[i] = width() - 1;
      }

      double rbuf[16], gbuf[16], bbuf[16];
      for (int i=0; i < 4; i++)
        for (int j=0; j < 4; j++)
        {
          rbuf[i + j * 4] = pixel(px[i], py[j]).r;
          gbuf[i + j * 4] = pixel(px[i], py[j]).g;
          bbuf[i + j * 4] = pixel(px[i], py[j]).b;
        }


      double tmpR[4], tmpG[4], tmpB[4];
      productMat14_44_(tmpR, fy, rbuf);
      productMat14_44_(tmpG, fy, gbuf);
      productMat14_44_(tmpB, fy, bbuf);

      double R = innerProduct_(4, tmpR, fx);
      double G = innerProduct_(4, tmpG, fx);
      double B = innerProduct_(4, tmpB, fx);

      UByte r, g, b;
      if (R < 0.0)
        r = 0;
      else if (R > 255.0)
        r = 255;
      else
        r = (UByte)R;

      if (G < 0.0)
        g = 0;
      else if (G > 255.0)
        g = 255;
      else
        g = (UByte)G;

      if (B < 0.0)
        b = 0;
      else if (B > 255.0)
        b = 255;
      else
        b = (UByte)B;

      pict->pixel(X, Y) = RgbColor(r, g, b);
    }
  }

  return pict;
}



//eof
