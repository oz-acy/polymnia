/**************************************************************************
 *
 *  pngoio.h
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PNG IO for part "polymnia"
 *  PNG�`���摜���o�̓N���X (libpng���b�p)
 *
 *  Classes defined in this file are inplemented with "libpng" library.
 *  The author thanks authors of "libpng."
 *
 *  ����
 *    2018.12.23 C++17����
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
  polymnia::Picture* load(const std::filesystem::path& path) override;
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
  polymnia::PictureIndexed* load(const std::filesystem::path& path) override;
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

  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
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

  bool
    save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;
};


#endif
//eof
