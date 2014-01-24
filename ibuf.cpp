/**************************************************************************
*
*  ibuf.cpp
*  by oZ/acy
*  (c) 2002-2009 oZ/acy. ALL RIGHTS RESERVED.
*
*  畫像バッファのクリッピング
*
*  last update: 2009.11.7
*
**************************************************************************/

#include "ibuf.h"
//#include <iostream>

polymnia::imp_::Clip_::Clip_(
  int ssx, int ssy, int ww, int hh, int ddx, int ddy, const Rect& mask)
  : flag_(false)
{
  if (ddx > mask.x2)
    return;
  if (ddy > mask.y2)
    return;
  if (ddx + ww <= mask.x1)
    return;
  if (ddy + hh <= mask.y1)
    return;

  if (ddx < mask.x1)
  {
    dx = mask.x1;
    ww -= mask.x1 - ddx;
    sx = ssx + mask.x1 - ddx;
  }
  else
  {
    dx = ddx;
    sx = ssx;
  }

  if (dx + ww > mask.x2)
    w = mask.x2 - dx + 1;
  else
    w = ww;


  if (ddy < mask.y1)
  {
    dy = mask.y1;
    hh -= mask.y1 - ddy;
    sy = ssy + mask.y1 - ddy;
  }
  else
  {
    dy = ddy;
    sy = ssy;
  }

  if (dy + hh > mask.y2)
    h = mask.y2 - dy + 1;
  else
    h = hh;

  flag_ = true;
}


//eof
