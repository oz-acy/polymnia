/**************************************************************************
 *
 *  picture.cpp
 *  by oZ/acy
 *  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class Picture, class PictureIndexed の生成・複製關聯
 *
 *  last update: 2011.9.8
 *
 *************************************************************************/

#include <algorithm>
#include "picture.h"


/*========================================================
 *  Picture::create()
 *  幅, 高さを指定して Picture オブジェクトを生成
 *======================================================*/
polymnia::Picture* polymnia::Picture::create(unsigned w, unsigned h) throw()
{
  try
  {
    return new Picture(w, h);
  }
  catch (std::bad_alloc)
  {
    return nullptr;
  }
}


/*=================================
 *  Picture::clone()
 *  Picture の自己複製
 *===============================*/
polymnia::Picture* polymnia::Picture::clone() const throw()
{
  using namespace std;

  Picture* res = create(w_ ,h_);
  if (res)
    copy(buf_, buf_ + h_ * offset_, res->buf_);
  return res;
}



/*==========================================================
 *  PictureIndexed::create()
 *  PictureIndexed オブジェクトを幅, 高さを指定して生成
 *========================================================*/
polymnia::PictureIndexed* polymnia::PictureIndexed::create(
  unsigned w, unsigned h) throw()
{
  try
  {
    return new PictureIndexed(w, h);
  }
  catch(std::bad_alloc)
  {
    return nullptr;
  }
}


/*====================================================
 *  PictureIndexed::clone()
 *  PictureIndexed オブジェクトの自己複製
 *==================================================*/
polymnia::PictureIndexed* polymnia::PictureIndexed::clone() const throw()
{
  using namespace std;

  PictureIndexed* res = create(w_, h_);
  if (res)
  {
    copy(buf_, buf_ + h_ * offset_, res->buf_);
    copy(pal_, pal_+256, res->pal_);
  }

  return res;
}



/*=======================================================================
 *  PictureIndexed::duplicatePicture()
 *  PictureIndexed から Picture オブジェクトを複製 (但し色形式は変更)
 *=====================================================================*/
polymnia::Picture* polymnia::PictureIndexed::duplicatePicture() const throw()
{
  Picture* pc = Picture::create(w_, h_);

  if (pc)
  {
    int area = w_ * h_;
    for (int i = 0; i < area; i++)
      pc->buffer()[i] = pal_[buf_[i]];

    //上記は下記からの書き直し
    //for (int y=0; y<h_; y++)
    //  for (int x=0; x<w_; x++)
    //    pc->pixel(x, y) = pal_[buf_[x+y*offset_]];
  }

  return pc;
}


//eof
