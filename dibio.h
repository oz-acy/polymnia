/**************************************************************************
 *
 *  dibio.h
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  DIB IO
 *  DIB�`���摜���o�͗p�N���X
 *
 *  ����
 *    2018.12.23 C++17���� path�̓n������̍X
 */

#ifndef INC_POLYMNIA_DIBIO_H___
#define INC_POLYMNIA_DIBIO_H___

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class DibLoader;
  class IndexedDibLoader;
  class DibSaver;
  class IndexedDibSaver;
}


/*-----------------------------------
 *  class DibLoader
 *  DIB �`���`���̃��[�_
 *  24bit RGB �`���Ƃ��Ď�荞��
 */
class polymnia::DibLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  DibLoader() {}
  ~DibLoader() {}
  polymnia::Picture* load(const std::filesystem::path& path) override;

//protected:
//  polymnia::Picture* load_(const char* path);
};


/*----------------------------------------------
 *  class IndexedDibLoader
 *  8bit �ȉ� indexed DIB �`���`���̃��[�_
 *  8bit indexed ���`���f�[�^�Ƃ��Ď�荞��
 */
class polymnia::IndexedDibLoader
: public polymnia::PictLoader<polymnia::PictureIndexed>
{
public:
  IndexedDibLoader() {}
  ~IndexedDibLoader() {}
  polymnia::PictureIndexed* load(const std::filesystem::path& path) override;
//protected:
//  polymnia::PictureIndexed* load_(const char* path);
};


/*---------------------------------
 *  class DibSaver
 *  24bit DIB �`���`���̃Z�[�o
 */
class polymnia::DibSaver : public polymnia::PictSaver<polymnia::Picture>
{
public:
  DibSaver() {}
  ~DibSaver() {}
  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;

//protected:
//  bool save_(const polymnia::Picture* p, const char* path);
};


/*-----------------------------------------
 *  class IndexedDibSaver
 *  8bit indexed DIB �`���`���̃Z�[�o
 */
class polymnia::IndexedDibSaver
: public polymnia::PictSaver<polymnia::PictureIndexed>
{
public:
  IndexedDibSaver() {}
  ~IndexedDibSaver() {}

  bool
    save(
    const polymnia::PictureIndexed* p,
    const std::filesystem::path& path) override;

//protected:
//  bool save_(const polymnia::PictureIndexed* p, const char* path);
};




#endif // INC_POLYMNIA_DIBIO_H___
