/*************************************************************************
 *
 *  tograyscale.cpp
 *  by oZ/acy
 *  (c) 2007-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  �O���C�X�P�[��������PictureIndexed�̐���
 *
 *  ����
 *    2008.04.20 �쐬
 *    2008.10.03 �C��
 *    2011.09.08 �C��
 *
 ************************************************************************/

#include "pictcvt.h"




/*================================================
 *  createPictureGrayScaleIndexed()
 *  �O���C�X�P�[�� + Indexed��
 */
polymnia::PictureIndexed*
polymnia::createPictureGrayScaleIndexed(const polymnia::Picture* src)
throw()
{
  using namespace themis;
  using namespace polymnia;

  /* PictureIndexed���� */
  PictureIndexed* pc = PictureIndexed::create(src->width(), src->height());
  if (!pc)
    return nullptr;

  /* �p���b�g�쐬 */
  for (int i = 0; i < 256; i++)
    pc->palette(i) = RgbColor((UByte)i, (UByte)i, (UByte)i);

  /* �O���C�X�P�[���� */

  for (int y = 0; y < src->height(); y++) {
    for (int x = 0; x < src->width(); x++) {
      RgbColor col = src->pixel(x, y);
      UByte l = (UByte)(col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140);
      pc->pixel(x, y) = l;
    }
  }

  return pc;
}




//eof
