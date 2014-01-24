/**************************************************************************
*
*  ibuf-imp_.h
*  by oZ/acy
*  (c) 2002-2008 oZ/acy.  ALL RIGHTS RESERVED.
*
*  implement of Image BUFfer template
*  畫像バッファクラステンプレート實裝部分
*
*  last update: 7 Nov MMIX
*
**************************************************************************/

#include <stack>
#include <cstring>
#include <cstdlib>


namespace polymnia{

namespace imp_{

/*----------------------------
*  class Clip
*  クリッピング處理用クラス
*---------------------------*/
class Clip_
{
public:
  int sx, sy;
  int w, h;
  int dx, dy;

private:
  bool flag_;

public:
  Clip_(int ssx, int ssy, int ww, int hh, int ddx, int ddy, const Rect& mask);
  operator bool() const { return flag_; }
};


}//end of namespace polymnia::imp_
}//end of namespace polymnia






/*=========================================
*  ImageBuffer<C_>::blt()
*  轉送
*========================================*/
template<class C_>
inline void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C_>* src, int sx, int sy,
  int w, int h)
{
  Rect rect(0, 0, w_ - 1, h_ - 1);
  blt(dx, dy, src, sx, sy, w, h, rect);
}

template<class C_>
void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C_>* src, int sx, int sy,
  int w, int h, const polymnia::Rect& mk)
{
  imp_::Clip_ clip(sx, sy, w, h, dx, dy, mk);

  if (clip)
  {
    int dp = clip.dx + clip.dy * offset_;
    int sp = clip.sx + clip.sy * src->offset_;
    for (int i=0; i<clip.h; i++, dp+=offset_, sp+=src->offset_)
      std::memcpy(&(buf_[dp]), &(src->buf_[sp]), clip.w * sizeof(C_));
  }
}


template<class C_>
inline void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C_>* src, int sx, int sy,
  int w, int h, const C_& tcol)
{
  Rect rect(0, 0, w_ - 1, h_ - 1);
  blt(dx, dy, src, sx, sy, w, h, tcol, rect);
}

template<class C_>
void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C_>* src, int sx, int sy,
  int w, int h, const C_& tcol, const polymnia::Rect& mk)
{
  imp_::Clip_ clip(sx, sy, w, h, dx, dy, mk);

  if (clip)
  {
    int dp = clip.dx + clip.dy * offset_;
    int sp = clip.sx + clip.sy * src->offset_;
    for (int i = 0; i < clip.h; i++, dp += offset_, sp += src->offset_)
      for (int j = 0; j < clip.w; j++)
      {
        if (src->buf_[sp + j] != tcol)
          buf_[dp + j] = src->buf_[sp + j];
      }
  }
}



template<class C_> template<class C2_>
inline void polymnia::ImageBuffer<C_>::blt(
  int dx,int dy, const polymnia::ImageBuffer<C2_>* src,
  int sx, int sy, int w, int h)
{
  Rect rect(0, 0, w_ - 1, h_ - 1);
  blt(dx, dy, src, sx, sy, w, h, rect);
}

template<class C_> template<class C2_>
void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
  int w, int h, const polymnia::Rect& mk)
{
  imp_::Clip_ clip(sx, sy, w, h, dx, dy, mk);

  if (clip)
  {
    const C2_* b = src->buffer();
    int dp = clip.dx + clip.dy * offset_;
    int sp = clip.sx + clip.sy * src->offset();
    for (int j=0; j<clip.h; j++, dp+=offset_, sp+=src->offset())
      for (int i=0; i<clip.w; i++)
        buf_[dp + i] = b[sp + i];
  }
}


template<class C_> template<class C2_>
inline void polymnia::ImageBuffer<C_>::blt(
  int dx,int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
  int w, int h, const C2_& tcol)
{
  Rect rect(0, 0, w_ - 1, h_ - 1);
  blt(dx, dy, src, sx, sy, w, h, tcol, rect);
}

template<class C_> template<class C2_>
void polymnia::ImageBuffer<C_>::blt(
  int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
  int w, int h, const C2_& tcol, const polymnia::Rect& mk)
{
  imp_::Clip_ clip(sx, sy, w, h, dx, dy, mk);

  if (clip)
  {
    const C2_* b = src->buffer();
    int dp = clip.dx + clip.dy * offset_;
    int sp = clip.sx + clip.sy * src->offset();
    for (int j = 0; j < clip.h; j++, dp += offset_, sp += src->offset())
      for (int i = 0; i < clip.w; i++)
        if (b[sp + i] != tcol)
          buf_[dp + i] = b[sp + i];
  }
}


/*===============================================
*  ImageBuffer<C_>::clear()
*  指定した色で全域をクリア
*==============================================*/
template<class C_>
void polymnia::ImageBuffer<C_>::clear(const C_& col)
{
  int dp = 0;
  for (int j = 0; j < h_; j++, dp += offset_)
    for (int i = 0; i < w_; i++)
      buf_[dp + i] = col;
}



/*===============================================
*  ImageBuffer<C>::addEffect()
*  畫像に効果處理をかける
*==============================================*/
/*
template<class C_>
inline void polymnia::ImageBuffer<C_>::addEffect(
  const typename polymnia::ImageBuffer<C_>::Effecter& efc)
{
  C_* src = new C_[h_ * offset_];
  std::memcpy(src, buf_, sizeof(C_) * h_ * offset_);
  efc(buf_, src, w_, h_, offset_);
  delete[] src;
}
*/


/*==========================================================
*  ImageBuffer<C>::line()
*  直線を引く
*  (Bresenham's Algorithm  參考: C MAGAZINE Dec. 2000)
*=========================================================*/
template<class C_>
void polymnia::ImageBuffer<C_>::line(int x1, int y1, int x2, int y2, const C_& col)
{
  int dx, dy, sx, sy;

  dx = x2 - x1;
  dy = y2 - y1;

  if (dx < 0)
  {
    sx = -1;
    dx = -dx;
  }
  else if (dx == 0)
    sx = 0;
  else
    sx = 1;

  if (dy < 0)
  {
    sy = -1;
    dy = -dy;
  }
  else if (dy == 0)
    sy = 0;
  else
    sy = 1;

  int xx1 = x1;
  int xx2 = x2;
  int yy1 = y1;
  int yy2 = y2;

  int e, i, p, q;
  if (dx >= dy)
  {
    e = -dx;
    int lx = (dx + 1) / 2;

    p = x1 + y1 * offset_;
    q = x2 + y2 * offset_;

    for (i = 0; i < lx; i++)
    {
      if (xx1 >= 0 && xx1 < w_ && yy1 >= 0 && yy1 < h_)
        buf_[p] = col;
      if (xx2 >= 0 && xx2 < w_ && yy2 >= 0 && yy2 < h_)
        buf_[q] = col;

      p += sx;
      q -= sx;
      xx1 += sx;
      xx2 -= sx;
      e += 2*dy;
      if (e>=0)
      {
        if (sy>0)
        {
          p += offset_;
          q -= offset_;
          yy1++;
          yy2--;
        }
        else if (sy<0)
        {
          p -= offset_;
          q += offset_;
          yy1--;
          yy2++;
        }

        e -= 2*dx;
      }
    }

    if (!(dx % 2) && xx1 >= 0 && xx1 < w_ && yy1 >= 0 && yy1 < h_)
      buf_[p] = col;
  }
  else
  {
    e = -dy;
    int ly = (dy + 1) / 2;
    p = x1 + y1 * offset_;
    q = x2 + y2 * offset_;

    for (i=0; i<ly; i++)
    {
      if (xx1 >= 0 && xx1 < w_ && yy1 >= 0 && yy1 < h_)
        buf_[p] = col;
      if (xx2 >= 0 && xx2 < w_ && yy2 >= 0 && yy2 < h_)
        buf_[q] = col;

      if(sy > 0)
      {
        p += offset_;
        q -= offset_;
        yy1++;
        yy2--;
      }
      else if (sy < 0)
      {
        p -= offset_;
        q += offset_;
        yy1--;
        yy2++;
      }

      e += 2 * dx;
      if (e>=0)
      {
        p += sx;
        q -= sx;
        xx1 += sx;
        xx2 -= sx;
        e -= 2*dy;
      }
    }

    if (!(dy % 2) && xx1 >= 0 && xx1 < w_ && yy1 >= 0 && yy1 < h_)
      buf_[p] = col;
  }
}


/*===============================================
*  ImageBuffer<C_>::box()
*  長方形を描く
*==============================================*/
template<class C_>
void polymnia::ImageBuffer<C_>::box
(int x1, int y1, int x2, int y2, const C_& col, bool fl)
{
  // 座標の簡単化
  if (x1>x2)
    std::swap(x1, x2);
  if (y1>y2)
    std::swap(y1, y2);

  // 全部が範圍外なら return
  if (x1>=w_ || x2<0 || y1>=h_ || y2<0)
    return;

  // 範圍内に切りつめた値
  int xx1, xx2, yy1, yy2;

  if (x1<0)
    xx1 = 0;
  else
    xx1 = x1;

  if (x2>=w_)
    xx2 = w_-1;
  else
    xx2 = x2;

  if (y1<0)
    yy1 = 0;
  else
    yy1 = y1;

  if (y2>=h_)
    yy2 = h_-1;
  else
    yy2 = y2;

  if (fl)
  { // 塗り潰し
    int i, j;
    int p = yy1 * offset_;
    int w = xx2 - xx1;
    for (i=yy1; i<=yy2; i++, p+=offset_)
      for (j=xx1; j<=xx2; j++)
        buf_[p+j] = col;
  }
  else
  {
    int i, p, q;

    // 横枠を描く
    p = xx1 + y1*offset_;
    q = xx1 + y2*offset_;
    if (y1>=0)
    {
      if (y2<h_)
        for (i=xx1; i<=xx2; i++,p++,q++)
          buf_[p] = buf_[q] = col;
      else
        for (i=xx1; i<=xx2; i++,p++)
          buf_[p] = col;
    }
    else
    {
      if (y2<h_)
        for (i=xx1; i<=xx2; i++,q++)
          buf_[q] = col;
    }

    // 縦枠を描く
    p = x1 + yy1*offset_;
    q = x2 + yy1*offset_;
    if (x1>=0)
    {
      if (x2<w_)
        for (i=yy1; i<=yy2; i++, p+=offset_, q+=offset_)
          buf_[p] = buf_[q] = col;
      else
        for (i=yy1; i<=yy2; i++,p+=offset_)
          buf_[p] = col;
    }
    else
    {
      if (x2<w_)
        for (i=yy1; i<=yy2; i++,q+=offset_)
          buf_[q] = col;
    }
  }
}


/*============================================
*  ImageBuffer<C>::ellipse()
*  楕円の描畫 (他のアルゴリズムを要檢討?)
*===========================================*/
/*     1 2
 *   3     4
 *   5     6       <-- コメント中で用いた番号
 *     7 8
 */
template<class C_>
void
polymnia::ImageBuffer<C_>::ellipse(
  int x, int y, int a, int b, const C_& col, bool fl)
{
  if (a==0 || b==0)
    return;

  int ysp, ysm, ytp, ytm;

  a = std::abs(a);
  b = std::abs(b);

  int p = 0;
  int q, e;
  if (a < b)
  {
    e = 2 - 3*b;
    q = b;
  }
  else
  {
    e = 2 - 3*a;
    q = a;
  }

  while (p <= q)
  {
    if (e < 0)
      e += (p << 2) + 6;
    else
    {
      e += ((p - q) << 2) + 10;
      q--;
    }

    int s, t, s2, t2;
    if (a < b)
    {
      s = p * a / b;   // 1, 2, 7, 8
      t = q;

      s2 = q * a / b;  // 3, 4, 5, 6 
      t2 = p;
    }
    else
    {
      s = p;
      t = q * b / a;

      s2 = q;
      t2 = p * b / a;
    }

    p++;


    if (x-s < w_ && x+s >= 0 && y-t < h_ && y+t >= 0)
    {
      if (fl)
      {
        int i = (x-s < 0) ? 0 : x-s;
        int j = (x+s+1 >= w_) ? w_-1 : x+s+1;
        if (y+t < h_)
        {
          int ytp = (y+t) * offset_;

          if (y-t >= 0)
          { // 1=>2, 7=>8
            int ytm = (y-t) * offset_;
            for ( ; i < j; ++i)
              buf_[i+ytp] = buf_[i+ytm] = col;
          }
          else
          { // 1=>2
            for ( ; i < j; ++i)
              buf_[i+ytp] = col;
          }
        }
        else
        {
          if (y-t >= 0)
          { //7->8
            int ytm = (y-t) * offset_;
            for ( ; i<j; i++)
              buf_[i+ytm] = col;
          }
        }
      }
      else
      {
        bool xs1 = (x-s) >= 0;
        bool xs2 = (x+s) < w_;

        if (y+t < h_)
        {
          ytp = (y+t) * offset_;
          if (xs1)
            buf_[x-s + ytp] = col;  //1
          if (xs2)
            buf_[x+s + ytp] = col;  //2
        }
        if (y-t >= 0)
        {
          ytm = (y-t) * offset_;
          if (xs1)
            buf_[x-s + ytm] = col;  //7
          if (xs2)
            buf_[x+s + ytm] = col;  //8
        }
      }
    }


    if (x-s2 < w_ && x+s2 >= 0 && y-t2 < h_ && y+t2 >= 0)
    {
      if (fl)
      {
        int i = (x-s2 < 0) ? 0 : x-s2;
        int j = (x+s2+1 >= w_) ? w_-1 : x+s2+1;
        if (y+t2 < h_)
        {
          int ytp = (y+t2) * offset_;

          if (y-t2 >= 0)
          { // 3=>4, 5=>6
            int ytm = (y-t2) * offset_;
            for ( ; i < j; ++i)
              buf_[i+ytp] = buf_[i+ytm] = col;
          }
          else
          { // 3=>4
            for ( ; i < j; ++i)
              buf_[i+ytp] = col;
          }
        }
        else
        {
          if (y-t2 >= 0)
          { //5=>6
            int ytm = (y-t2) * offset_;
            for ( ; i<j; i++)
              buf_[i+ytm] = col;
          }
        }
      }
      else
      {
        bool xs1 = (x-s2) >= 0;
        bool xs2 = (x+s2) < w_;

        if (y+t2 < h_)
        {
          ytp = (y+t2) * offset_;
          if (xs1)
            buf_[x-s2 + ytp] = col;  //3
          if (xs2)
            buf_[x+s2 + ytp] = col;  //4
        }
        if (y-t2 >= 0)
        {
          ytm = (y-t2) * offset_;
          if (xs1)
            buf_[x-s2 + ytm] = col;  //5
          if (xs2)
            buf_[x+s2 + ytm] = col;  //6
        }
      }
    }

  }// while
}


/*====================================================
*  ImageBuffer<C>::paintFill()
*  所謂塗り潰し
*===================================================*/
template<class C_>
void polymnia::ImageBuffer<C_>::paintFill(int x, int y, const C_& col)
{
  if (x < 0 || y < 0 || x >= w_ || y >= h_)
    return;

  std::stack<int> p_stc;
  unsigned pos = x + y*offset_;

  p_stc.push(pos);
  C_ p_col = buf_[pos];
  if (p_col==col)
    return;

  while (!p_stc.empty())
  {
    int pt = p_stc.top();
    p_stc.pop();

    int xx = pt % offset_;
    int yy = pt / offset_;

    bool ful, fur;
    if (yy > 0 && buf_[pt-offset_]==p_col)
    {
      p_stc.push(pt-offset_);
      ful = fur = true;
    }
    else
      ful = fur = false;

    bool fdl, fdr;
    if (yy < h_ - 1 && buf_[pt+offset_]==p_col)
    {
      p_stc.push(pt+offset_);
      fdl = fdr = true;
    }
    else
      fdl = fdr = false;


    int L = pt - 1;
    int l = xx - 1;
    int R = pt + 1;
    int r = xx + 1;

    buf_[pt] = col;

    while (l >= 0 && buf_[L]==p_col)
    {
      if (L-offset_ >= 0)
      {
        if (ful && buf_[L-offset_] != p_col)
          ful = false;
        else if (!ful && buf_[L-offset_] == p_col)
        {
          ful = true;
          p_stc.push(L-offset_);
        }
      }

      if (L+offset_ < h_*offset_)
      {
        if (fdl && buf_[L+offset_] != p_col)
          fdl = false;
        else if (!fdl && buf_[L+offset_] == p_col)
        {
          fdl = true;
          p_stc.push(L+offset_);
        }
      }

      buf_[L] = col;
      --L;
      --l;
    }

    while (r < w_ && buf_[R]==p_col)
    {
      if (R-offset_ >= 0)
      {
        if (fur)
          fur = false;
        else if (buf_[R-offset_]==p_col)
        {
          fur = true;
          p_stc.push(R - offset_);
        }
      }

      if (R+offset_ < h_*offset_)
      {
        if (fdr && buf_[R+offset_] != p_col)
          fdr = false;
        else if (!fdr && buf_[R+offset_] == p_col)
        {
          fdr = true;
          p_stc.push(R + offset_);
        }
      }

      buf_[R] = col;
      ++R;
      ++r;
    }
  }
}



//eof
