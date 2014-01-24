/**************************************************************************
 *  reduce.cpp
 *
 *  (C) 2003-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Picture�p�k�����[�`��
 *
 *  ����
 *    27 Nov MMIV   resize.cpp ����k�����p���[�`����؂蕪��
 *    27 Feb MMVIII LIBPOLYMNIA�Ɉړ�
 *    20 Apr MMVIII Picture��LIBPOLYMNIA�Ɉړ��������Ƃɂ��C��
 *     8 Sep MMXI   NULL �� nullptr �ɏC��(C++11)
 *    28 Jul MMXII  �o�O�C��(�������s���A�N�Z�X)
 *************************************************************************/

#include "pictcvt.h"


namespace {


polymnia::RgbColor
toSmall__(
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

  double S = 0.0;  // �`�ʐ�
  double R = 0.0;
  double G = 0.0;
  double B = 0.0;

  for (int y = sy; y < ey; y++)
  {
    double e1 = (double)y;
    double e2 = (double)(y + 1);
    if (e1 < y1)
      e1 = y1;  // ��[
    if (e2 > y2)
      e2 = y2;  // ���[

    double ph = e2 - e1;  // ����

    for (int x = sx; x < ex; x++)
    {
      double e3 = (double)x;
      double e4 = (double)(x + 1);
      if (e3 < x1)
        e3 = x1;  // ���[
      if (e4 > x2)
        e4 = x2;  // �E�[

      double pw = e4 - e3;  // ��

      double ss = ph * pw;  // �ʐ�
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

  return RgbColor((UByte)(R + 0.5), (UByte)(G + 0.5), (UByte)(B + 0.5));
}


}//end of namespace


/*================================================================
 *  createReducedPicture()
 *  �`���k�����[�`��
 *==============================================================*/
polymnia::Picture* 
polymnia::createReducedPicture(int w, int h, const polymnia::Picture* src)
throw()
{
  using namespace polymnia;

  Picture* pict = Picture::create(w, h);
  if (!pict)
    return nullptr;

  double dw = (double)(src->width()) / (double)w;
  double dh = (double)(src->height()) / (double)h;

  for (int Y = 0; Y < h; Y++)
  {
    double y1 = (double)Y * dh;
    double y2 = (double)(Y + 1) * dh;
    
    if (y2 > src->height())
      break;

    for (int X = 0; X < w; X++)
    {
      double x1 = (double)X * dw;
      double x2 = (double)(X + 1) * dw;

      if (x2 > src->width())
        break;

      pict->pixel(X, Y) = toSmall__(src, x1, y1, x2, y2);
    }
  }

  return pict;
}



//eof
