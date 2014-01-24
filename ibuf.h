/**************************************************************************
*
*  ibuf.h
*  by oZ/acy
*  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
*
*  Image BUFfer template
*  �`���o�b�t�@�̃N���X�e���v���[�g
*
*  last update: 2011.11.7
**************************************************************************/

#ifndef INC_POLYMNIA_IMAGEBUFFER_H___
#define INC_POLYMNIA_IMAGEBUFFER_H___

#include <boost/utility.hpp>
#include <themis/except.h>


namespace polymnia
{

template<class C_> class ImageBuffer;

/*--------------------------------------------------
*  struct Point
* �u�y�v�\����
*-------------------------------------------------*/
struct Point
{
  int x, y;

  Point() = default;
  Point(int xx, int yy) : x(xx), y(yy) {}
};


/*--------------------------------------------------
*  struct Rect
*  �u�����`�v�\����
*-------------------------------------------------*/
struct Rect
{
  int x1, y1, x2, y2;

  Rect() = default;
  Rect(int x, int y, int xx, int yy) : x1(x), y1(y), x2(xx), y2(yy) {}
};




/*---------------------------------
*  class RangeOverException
*  �͚���E��O
*--------------------------------*/
class RangeOverException : public themis::EmptyException
{
public:
  const char* what() const throw()
  {
    static const char rstr[] = "polymnia::RangeOverException";
    return rstr;
  }
};


}//end of namespace polymnia




/*----------------------------------------------------
*  ImageBuffer<C_>
*  �`���o�b�t�@���N���X�e���v���[�g
*  �C���X�^���X���͕s��
*---------------------------------------------------*/
template<class C_>
class polymnia::ImageBuffer : boost::noncopyable
{
public:
  typedef C_ ColorType;

  //-------------------------------------
  //  class Effecter
  //  �`�����ʒǉ��p�̒��ۊ��N���X
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

  int width() const throw() { return w_; }
  int height() const throw() { return h_; }
  int offset() const throw() { return offset_; }

  C_* buffer() throw() { return buf_; }
  const C_* buffer() const throw() { return buf_; }


  C_& pixel(int x, int y) throw() { return buf_[x+y*offset_]; }
  const C_& pixel(int x, int y) const throw() { return buf_[x+y*offset_]; }


  C_& at(int x, int y) throw(polymnia::RangeOverException)
  {
    if (x<0)
      throw polymnia::RangeOverException();
    if (x>=w_)
      throw polymnia::RangeOverException();
    if (y<0)
      throw polymnia::RangeOverException();
    if (y>=h_)
      throw polymnia::RangeOverException();

    return buf_[x+y*offset_];
  }

  const C_& at(int x, int y) const throw(polymnia::RangeOverException)
  {
    if (x<0)
      throw polymnia::RangeOverException();
    if (x>=w_)
      throw polymnia::RangeOverException();
    if (y<0)
      throw polymnia::RangeOverException();
    if (y>=h_)
      throw polymnia::RangeOverException();

    return buf_[x+y*offset_];
  }


  //======================================================================
  //  ���`�`��
  //======================================================================
  void line(int x1, int y1, int x2, int y2, const C_& col);
  void box(int x1, int y1, int x2, int y2, const C_& col, bool fl=false);
  void ellipse(int x, int y, int a, int b, const C_& col, bool fl=false);

  void circle(int x, int y, int r, const C_& col, bool fl=false)
  {
    ellipse(x, y, r, r, col, fl);
  }


  //=======================================================
  //  �h��ׂ�
  //=======================================================
  void paintFill(int x, int y, const C_& col);


  //==============================================
  //  �S�`�ʓh��ׂ�
  //==============================================
  void clear(const C_& col);


  //===================================================================
  //  �z��
  //  tcol �����̂��̂�, tcol �𓧖��F�ƊŘ􂵂��z��
  //  mk �����̂��̂�, mk ���}�X�N�Ƃ����z��
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
  //  �G�t�F�N�g��������
  //===================================================
  //void addEffect(const Effecter& efc);
};


#include "ibuf-imp_.h"


#endif // INC_POLYMNIA_IMAGEBUFFER_H___
