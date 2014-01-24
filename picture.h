/**************************************************************************
 *
 *  picture.h
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PICTURE buffer (RGB24bit, 256palRGB)
 *  RGB24bit と RGB256パレット用の畫像バッファクラス
 *
 *  last update: 2012.3.1
 *
 ************************************************************************/

#ifndef INC_POLYMNIA_PICTURE_H___
#define INC_POLYMNIA_PICTURE_H___

#include <string>
#include <cstring>
#include "ibuf.h"
#include "rgb.h"


namespace polymnia
{
  class Picture;
  class PictureIndexed;
}


/*----------------------------
*  class Picture
*  RGB24bit用畫像バッファ
*---------------------------*/
class polymnia::Picture : public polymnia::ImageBuffer<polymnia::RgbColor>
{
protected:
  Picture(unsigned w, unsigned h)
    : polymnia::ImageBuffer<polymnia::RgbColor>(w, h, w)
    { buf_ = new polymnia::RgbColor[w_*h_]; }

public:
  ~Picture() { delete[] buf_; }
  static Picture* create(unsigned w, unsigned h) throw();
  Picture* clone() const throw();
};




/*---------------------------------
*  class PictureIndexed
*  RGB256パレット用畫像バッファ
*--------------------------------*/
class polymnia::PictureIndexed : public polymnia::ImageBuffer<themis::UByte>
{
protected:
  polymnia::RgbColor pal_[256];

  PictureIndexed(unsigned w, unsigned h)
    : polymnia::ImageBuffer<themis::UByte>(w, h, w)
    { buf_ = new themis::UByte[w_*h_]; }


public:
  ~PictureIndexed() { delete[] buf_; }

  static PictureIndexed* create(unsigned w, unsigned h) throw();

  PictureIndexed* clone() const throw();


  // パレットへのアクセス
  polymnia::RgbColor& palette(int id) throw() { return pal_[id]; }
  const polymnia::RgbColor& palette(int id) const throw() { return pal_[id]; }
  polymnia::RgbColor* paletteBuffer() throw() { return pal_; }
  const polymnia::RgbColor* paletteBuffer() const throw() { return pal_; }

  // 同内容のPictureを生成
  polymnia::Picture* duplicatePicture() const throw();
};



#endif // INC_POLYMNIA_PICTURE_H___
