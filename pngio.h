/**************************************************************************
 *
 *  pngoio.h
 *  by oZ/acy
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PNG IO for part "polymnia"
 *  PNG�`���摜���o�̓N���X (libpng���b�p)
 *
 *  Classes defined in this file are used "libpng" library.
 *  The author thanks authors of "libpng."
 *
 *  last update: 2012.3.1
 *
 *************************************************************************/
#ifndef INC_POLYMNIA_PNGIO_H__
#define INC_POLYMNIA_PNGIO_H__

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class PngLoader;
  class IndexedPngLoader;
  class PngSaver;
  class IndexedPngSaver;
}


/*----------------------------------
 *  PngLoader
 *  PNG�`�����[�_
 *  RGB24bit�摜�Ƃ��Ď�荞��
 */
class polymnia::PngLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  PngLoader() {}
  ~PngLoader() {}

protected:
  polymnia::Picture* load_(const char* path);
};


/*-----------------------------------------------
 *  IndexedPngLoader
 *  PNG�`�����[�_ (8bit�ȉ���indexed)
 *  8bit indexed �摜�Ƃ��Ď�荞��
 */
class polymnia::IndexedPngLoader
: public polymnia::PictLoader<polymnia::PictureIndexed>
{
public:
  IndexedPngLoader() {}
  ~IndexedPngLoader() {}

protected:
  polymnia::PictureIndexed* load_(const char* path);
};



/*-------------------------------------------
 *  PngSaver
 *  24bit RGB PNG�`���̃Z�[�o
 *  �C���^�[���X�̗L��/������ݒ�\
 */
class polymnia::PngSaver : public polymnia::PictSaver<polymnia::Picture>
{
private:
  bool interlace;

public:
  PngSaver() : interlace(false) {}
  ~PngSaver() {}
  void enableInterlace() { interlace = true; }
  void disableInterlace() { interlace = false; }

protected:
  bool save_(const polymnia::Picture* p, const char* path);
};


/*--------------------------------------------------
 *  IndexedPngSaver
 *  8bit indexed PNG�`���̃Z�[�o
 *  �C���^�[���X�̗L��/����, ���ߐF��ݒ�\
 */
class polymnia::IndexedPngSaver
: public polymnia::PictSaver<polymnia::PictureIndexed>
{
private:
  bool interlace;
  bool trans;
  themis::UByte paltp;

public:
  IndexedPngSaver() : interlace(false), trans(false), paltp(0) {}
  ~IndexedPngSaver() {}
  void enableInterlace() { interlace = true; }
  void disableInterlace() { interlace = false; }
  void enableTransparent(themis::UByte pl) { trans = true; paltp = pl; }
  void disableTransparent() { trans = false; }

protected:
  bool save_(const polymnia::PictureIndexed* p, const char* path);
};


#endif
//eof
