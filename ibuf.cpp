/*
 * Copyright 2002-2021 oZ/acy (名賀月晃嗣)
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
 * @file ibuf.cpp
 * @author oZ/acy
 * @brief 畫像バッファのクリッピング
 *
 * @date 2009.11.7 修正
 *
 */
#include "ibuf.h"

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

  if (ddx < mask.x1) {
    dx = mask.x1;
    ww -= mask.x1 - ddx;
    sx = ssx + mask.x1 - ddx;
  }
  else {
    dx = ddx;
    sx = ssx;
  }

  if (dx + ww > mask.x2)
    w = mask.x2 - dx + 1;
  else
    w = ww;


  if (ddy < mask.y1) {
    dy = mask.y1;
    hh -= mask.y1 - ddy;
    sy = ssy + mask.y1 - ddy;
  }
  else {
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
