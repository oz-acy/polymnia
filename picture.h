/**************************************************************************
 *
 *  picture.h
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PICTURE buffer (RGB24bit, 256palRGB)
 *  RGB24bit と RGB256パレット用の畫像バッファクラス
 *
 *  履歴
 *    2016.3.2  C++11對應(假)
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
    { buf_ = new polymnia::RgbColor[w_ * h_]; }

public:
  ~Picture() { delete[] buf_; }
  static Picture* create(unsigned w, unsigned h) noexcept;
  Picture* clone() const noexcept;
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

  static PictureIndexed* create(unsigned w, unsigned h) noexcept;

  PictureIndexed* clone() const noexcept;


  // パレットへのアクセス
  polymnia::RgbColor& palette(int id) noexcept { return pal_[id]; }
  const polymnia::RgbColor& palette(int id) const noexcept { return pal_[id]; }
  polymnia::RgbColor* paletteBuffer() noexcept { return pal_; }
  const polymnia::RgbColor* paletteBuffer() const noexcept { return pal_; }

  // 同内容のPictureを生成
  polymnia::Picture* duplicatePicture() const noexcept;
};



#endif // INC_POLYMNIA_PICTURE_H___
