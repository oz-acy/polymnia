/**************************************************************************
 *
 *  pictcvt.h
 *
 *  by oZ/acy
 *  (c) 2008-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Picture Convert
 *
 *  履歴
 *    2016.3.2  C++11對應
 *
 *************************************************************************/

#ifndef INC_POLYMNIA_PICTURE_CONVERT_HEADER_
#define INC_POLYMNIA_PICTURE_CONVERT_HEADER_

#include "picture.h"

namespace polymnia {

void convertToGrayScaled(Picture* tgt) noexcept;

PictureIndexed* createPictureGrayScaleIndexed(const Picture* src) noexcept;
PictureIndexed* reducePictureColors(const Picture* src);

Picture* createReducedPicture(int w, int h, const Picture* src);
Picture* createMagnifiedPicture(int w, int h, const Picture* src);


}// end of namespace polymnia


#endif // INC_POLYMNIA_PICTURE_CONVERT_HEADER_
