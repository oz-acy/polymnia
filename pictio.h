/**************************************************************************
 *
 *  pictio.h
 *  by oZ/acy
 *  (c) 2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  PICTue buffer IO
 *  �`���o�b�t�@�̃��[�h�A�Z�[�u�p���N���X�e���v���[�g
 *
 *  last update: 2012.3.1
 *
 ************************************************************************/

#ifndef INC_POLYMNIA_PICTURE_IO_H___
#define INC_POLYMNIA_PICTURE_IO_H___

#include <string>
#include <boost/utility.hpp>


namespace polymnia
{
  template <class P> class PictLoader;
  template <class P> class PictSaver;
}


/*---------------------------------
 *  PictLoader<P_>
 *  �`�����[�h�p�N���X���
 */
template<class P_>
class polymnia::PictLoader : boost::noncopyable
{
public:
  PictLoader() throw() {}
  virtual ~PictLoader() {}

  P_* load(const char* path) { return load_(path); }
  P_* load(const std::string& path) { return load(path.c_str()); }

protected:
  virtual P_* load_(const char* path) =0;
};


/*---------------------------------
 *  PictSaver<P_>
 *  �`���Z�[�u�p�N���X���
 */
template<class P_>
class polymnia::PictSaver : boost::noncopyable
{
public:
  PictSaver() throw() {}
  virtual ~PictSaver() {}

  bool save(const P_* p, const char* path) { return save_(p, path); }

  bool save(const P_* p, const std::string& path)
    { return save_(p, path.c_str()); }

protected:
  virtual bool save_(const P_* p, const char* path) =0;
};




#endif // INC_POLYMNIA_PICTURE_IO_H___
