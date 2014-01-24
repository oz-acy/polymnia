/*************************************************************************
*
*  colcnv.cpp
*  by oZ/acy
*  (c) 2007-2008 oZ/acy.  ALL RIGHTS RESERVED.
*
*  Picture �̐F�̊����[�`��
*
*  last update: 2008.4.20
*
*************************************************************************/

#include "pictcvt.h"


/*================================================
*  convertToGrayScaled()
*  �O���C�X�P�[����
*/
void polymnia::convertToGrayScaled(polymnia::Picture* tgt) throw()
{
  using namespace themis;

  for (int y = 0; y < tgt->height(); y++) {
    for (int x = 0; x < tgt->width(); x++) {
      RgbColor col = tgt->pixel(x, y);
      UByte l = (UByte)(col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140);
      tgt->pixel(x, y) = RgbColor(l, l, l);
    }
  }
}




//eof
