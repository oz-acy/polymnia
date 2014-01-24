/**************************************************************************
*
*  pictcvt.h
*
*  by oZ/acy
*  (c) 2008 oZ/acy.  ALL RIGHTS RESERVED.
*
*  Picture Convert
*
*  last update: 2008.4.20
*
*************************************************************************/

#ifndef INC_POLYMNIA_PICTURE_CONVERT_HEADER_
#define INC_POLYMNIA_PICTURE_CONVERT_HEADER_

#include "picture.h"

namespace polymnia {

void convertToGrayScaled(Picture* tgt) throw();

PictureIndexed* createPictureGrayScaleIndexed(const Picture* src) throw();
PictureIndexed* reducePictureColors(const Picture* src) throw();

Picture* createReducedPicture(int w, int h, const Picture* src) throw();
Picture* createMagnifiedPicture(int w, int h, const Picture* src) throw();


}// end of namespace polymnia


#endif // INC_POLYMNIA_PICTURE_CONVERT_HEADER_
