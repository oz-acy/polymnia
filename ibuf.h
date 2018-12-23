/**************************************************************************
 *
 *  ibuf.h
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Image BUFfer template
 *  畫像バッファのクラステンプレート
 *
 *  履歴
 *    2016.3.2  C++11/14對應(假)
 *    2018.12.21  C++17對應(假)
 *************************************************************************/

#ifndef INC_POLYMNIA_IMAGEBUFFER_H___
#define INC_POLYMNIA_IMAGEBUFFER_H___

#include <boost/utility.hpp>
#include <themis/except.h>


namespace polymnia
{

template<class C_> class ImageBuffer;

/*--------------------------------------------------
*  struct Point
* 「點」構造体
*-------------------------------------------------*/
struct Point
{
  int x, y;

  Point() = default;
  constexpr Point(int xx, int yy) : x(xx), y(yy) {}
};


/*--------------------------------------------------
*  struct Rect
*  「長方形」構造体
*-------------------------------------------------*/
struct Rect
{
  int x1, y1, x2, y2;

  Rect() = default;
  constexpr Rect(int x, int y, int xx, int yy) : x1(x), y1(y), x2(xx), y2(yy) {}
};




/*---------------------------------
*  class RangeOverException
*  範圍逸脱例外
*--------------------------------*/
class RangeOverException : public themis::EmptyException
{
public:
  const char* what() const noexcept
  {
    static const char rstr[] = "polymnia::RangeOverException";
    return rstr;
  }
};


}//end of namespace polymnia




/*----------------------------------------------------
*  ImageBuffer<C_>
*  畫像バッファ基底クラステンプレート
*  インスタンス化は不可
*---------------------------------------------------*/
template<class C_>
class polymnia::ImageBuffer : boost::noncopyable
{
public:
  typedef C_ ColorType;

  //-------------------------------------
  //  class Effecter
  //  畫像効果追加用の抽象基底クラス
  //-------------------------------------
  //class Effecter
  //{
  //public:
  //  Effecter() {}
  //  virtual ~Effecter() {}
  //
  //  virtual void operator() (
  //    C_* buf, const C_* src, int w, int h, int offset) const =0;
  //};

private:
  typedef polymnia::ImageBuffer<C_>& Ref_;
  typedef const polymnia::ImageBuffer<C_>& CR_;
  typedef const polymnia::ImageBuffer<C_>* CP_;


protected:
  C_* buf_;
  int w_;
  int h_;
  int offset_;

  ImageBuffer(int w, int h, int o) : buf_(nullptr), w_(w), h_(h), offset_(o) {}

public:
  virtual ~ImageBuffer() {}

  int width() const noexcept { return w_; }
  int height() const noexcept { return h_; }
  int offset() const noexcept { return offset_; }

  C_* buffer() noexcept { return buf_; }
  const C_* buffer() const noexcept { return buf_; }


  C_& pixel(int x, int y) noexcept { return buf_[x + y * offset_]; }
  const C_& pixel(int x, int y) const noexcept { return buf_[x + y * offset_]; }


  C_& at(int x, int y) //throw(polymnia::RangeOverException)
  {
    if (x < 0)
      throw polymnia::RangeOverException();
    if (x >= w_)
      throw polymnia::RangeOverException();
    if (y<0)
      throw polymnia::RangeOverException();
    if (y >= h_)
      throw polymnia::RangeOverException();

    return buf_[x + y * offset_];
  }

  const C_& at(int x, int y) const //throw(polymnia::RangeOverException)
  {
    if (x < 0)
      throw polymnia::RangeOverException();
    if (x >= w_)
      throw polymnia::RangeOverException();
    if (y < 0)
      throw polymnia::RangeOverException();
    if (y >= h_)
      throw polymnia::RangeOverException();

    return buf_[x + y * offset_];
  }


  //======================================================================
  //  圖形描写
  //======================================================================
  void line(int x1, int y1, int x2, int y2, const C_& col);
  void box(int x1, int y1, int x2, int y2, const C_& col, bool fl=false);
  void ellipse(int x, int y, int a, int b, const C_& col, bool fl=false);

  void circle(int x, int y, int r, const C_& col, bool fl=false)
  {
    ellipse(x, y, r, r, col, fl);
  }


  //=======================================================
  //  塗り潰し
  //=======================================================
  void paintFill(int x, int y, const C_& col);


  //==============================================
  //  全畫面塗り潰し
  //==============================================
  void clear(const C_& col);


  //===================================================================
  //  轉送
  //  tcol 附きのものは, tcol を透明色と看做して轉送
  //  mk 附きのものは, mk をマスクとして轉送
  //===================================================================
  void blt(int dx, int dy, CP_ src, int sx, int sy, int w, int h);
  void blt(
    int dx, int dy, CP_ src, int sx, int sy, int w, int h,
    const polymnia::Rect& mk);

  void blt(
    int dx, int dy, CP_ src, int sx, int sy, int w, int h, const C_& tcol);
  void blt(
    int dx, int dy, CP_ src, int sx, int sy, int w, int h, const C_& tcol,
    const polymnia::Rect& mk);

  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
    int w, int h);

  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src,
    int sx, int sy, int w, int h, const polymnia::Rect& mk);

  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
    int w, int h, const C2_& tcol);

  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src,
    int sx, int sy, int w, int h, const C2_& tcol, const polymnia::Rect& mk);


  //===================================================
  //  エフェクトをかける
  //===================================================
  //void addEffect(const Effecter& efc);
};


#include "ibuf-imp_.h"


#endif // INC_POLYMNIA_IMAGEBUFFER_H___
