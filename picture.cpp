/**************************************************************************
 *
 *  picture.cpp
 *  by oZ/acy
 *  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class Picture, class PictureIndexed �̐����E����萗�
 *
 *  last update: 2011.9.8
 *
 *************************************************************************/

#include <algorithm>
#include "picture.h"


/*========================================================
 *  Picture::create()
 *  ��, �������w�肵�� Picture �I�u�W�F�N�g�𐶐�
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
 *  Picture �̎��ȕ���
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
 *  PictureIndexed �I�u�W�F�N�g��, �������w�肵�Đ���
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
 *  PictureIndexed �I�u�W�F�N�g�̎��ȕ���
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
 *  PictureIndexed ���� Picture �I�u�W�F�N�g�𕡐� (�A���F�`���͕ύX)
 *=====================================================================*/
polymnia::Picture* polymnia::PictureIndexed::duplicatePicture() const throw()
{
  Picture* pc = Picture::create(w_, h_);

  if (pc)
  {
    int area = w_ * h_;
    for (int i = 0; i < area; i++)
      pc->buffer()[i] = pal_[buf_[i]];

    //��L�͉��L����̏�������
    //for (int y=0; y<h_; y++)
    //  for (int x=0; x<w_; x++)
    //    pc->pixel(x, y) = pal_[buf_[x+y*offset_]];
  }

  return pc;
}


//eof
