/**************************************************************************
 *
 *  dibin.cpp
 *
 *  (c) 2002-2012 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  DIB INput
 *  DIB�`���摜���͗p�N���X����
 *
 *  last update: 2012.3.1
 *
 *************************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
#include <new>
#include "dibio.h"


namespace
{

/*---------------------------------------------------
*  class Error
*  �����I�ɗp����G���[�n���h���N���X
*--------------------------------------------------*/
class Error {};


/*----------------------------------------------
*  struct Info_
*  DIB�t�@�C�����(�̈ꕔ)
*---------------------------------------------*/
struct Info_
{
  int w;    //��
  int h;    //����
  int bit;  //Bit per Pixel
  int npal; //�p���b�g��
};


//////////////////////////////////////////////////
// �t�@�C���擪���̓ǂݍ��ݕ���
//////////////////////////////////////////////////

/*==============================================
*  readHeader__()
*  DIB�w�b�_�ǂݍ��� 
*=============================================*/
bool readHeader__(std::istream& is)
{
  using namespace themis;

  UWord w;
  UDWord d;

  // �}�W�b�N�i���o�[("BM")�̓ǂݍ��݂Əƍ�
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;
  if (w!=*(UWord*)("BM"))
    return false;

  // �t�@�C���T�C�Y�ǂݍ���
  if (!is.read((char*)&d, sizeof(UDWord)))
    return false;

  // �\��σ��[�h(2��)�ǂݍ���
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;

  // BitData�ւ̃I�t�Z�b�g�ǂݍ���
  if (!is.read((char*)&d, sizeof(UDWord)))
    return false;

  return true;
}

/*========================================================
*  readInfo__()
*  BMP-Info�w�b�_��椂ݍ��� (struct Info �ɒl��ݒ�)
*=======================================================*/
bool readInfo__(std::istream& is, Info_* pi)
{
  using namespace themis;

  UWord w;
  UDWord u;
  SDWord d;

  // InfoHeader �̃T�C�Y
  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;

  // �`����
  if (!is.read((char*)&d, sizeof(SDWord)))
    return false;
  pi->w = d;

  // �`������
  if (!is.read((char*)&d, sizeof(SDWord)))
    return false;
  pi->h = d;


  if (!is.read((char*)&w, sizeof(UWord)))
    return false;

  // bits per pixel
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;
  pi->bit = w;

  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;
  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;
  if (!is.read((char*)&d, sizeof(SDWord)))
    return false;
  if (!is.read((char*)&d, sizeof(SDWord)))
    return false;

  // �p���b�g��
  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;
  pi->npal = u;

  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;

  return true;
}

/*==============================================
*  readPalette__()
*  �p���b�g�ǂݍ���
*=============================================*/
bool readPalette__(std::istream& is, polymnia::RgbColor pal[], int npal)
{
  using namespace themis;

  struct
  {
    UByte b;
    UByte g;
    UByte r;
    UByte v;
  } quad[256];

  if (!is.read((char*)quad, sizeof(UByte)*4*npal))
    return false;

  for (int i = 0; i < npal; i++)
    pal[i] = polymnia::RgbColor(quad[i].r, quad[i].g, quad[i].b);

  return true;
}

/*===================================================
*  getBufSize__()
*  Bitmap �� 1line �̃o�b�t�@�������߂�
*==================================================*/
inline int getBufSize__(int l)
{
  return (l+3) & ~3;
}


/*=================================================
*  �p���b�g -> RGB �|��
*================================================*/
void
palpaint__(
  polymnia::RgbColor dbuf[], const themis::UByte sbuf[], int w,
  const polymnia::RgbColor pal[]) throw()
{
  for (int i = 0; i < w; i++)
    dbuf[i] = pal[sbuf[i]];
}

/*=============================================
*  read01_oneline__()
*  1bit bitmap �f�[�^��1���C���ǂݍ���
*============================================*/
void
read01_oneline__(themis::UByte dbuf[], themis::UByte sbuf[], int w) throw()
{
  for (int i = 0; i < w / 8; i++)
    for (int j = 7; j >= 0; j--)
    {
      dbuf[i * 8 + j] = (themis::UByte)(sbuf[i] & 1);
      sbuf[i] >>= 1;
    }

  int dw = w & 7;
  w = w & ~7;
  for (int i = dw - 1; i >= 0; i--)
  {
    dbuf[w + i] = (themis::UByte)(sbuf[w >> 3] & 1);
    sbuf[w >> 3] >>= 1;
  }
}


/*================================================
*  read04_oneline__()
*  4bit bitmap �f�[�^��1���C���ǂݍ���
*===============================================*/
void
read04_oneline__(themis::UByte dbuf[], themis::UByte sbuf[], int w) throw()
{
  using namespace themis;

  for (int i = 0; i < w / 2; i++)
  {
    dbuf[i*2+1] = (UByte)(sbuf[i] & 0x0F);
    dbuf[i*2] = (UByte)((sbuf[i] >> 4) & 0x0F);
  }

  if (w & 1)
    dbuf[w-1] = (UByte)(sbuf[w / 2] & 0x0F);
}


/*====================================================
*  read01bit__()
*  1Bit Bitmap �f�[�^�ǂݍ���(24bit�p)
*===================================================*/
bool
read01bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
throw()
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width() + 7) / 8);

  UByte* linebuf;
  UByte* palbuf;
  try
  {
    linebuf = new UByte[bufsize];
    palbuf = new UByte[pct->width()];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  int j;
  for (j=0; j < pct->height(); j++, pctbuf += pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    read01_oneline__(palbuf, linebuf, pct->width());
    palpaint__(pctbuf, palbuf, pct->width(), pal);
  }

  delete[] linebuf;
  delete[] palbuf;
  return true;
}


/*=============================================
*  read04bit__()
*  4Bit Bitmap �f�[�^�ǂݍ���(24bit�p)
*============================================*/
bool
read04bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
throw()
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width()+1)/2);

  UByte* linebuf;
  UByte* palbuf;
  try
  {
    linebuf = new UByte[bufsize];
    palbuf = new UByte[pct->width()];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height()-1);
  int j;
  for (j=0; j<pct->height(); j++, pctbuf+=pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    read04_oneline__(palbuf, linebuf, pct->width());
    palpaint__(pctbuf, palbuf, pct->width(), pal);
  }

  delete[] linebuf;
  delete[] palbuf;
  return true;
}

/* 8Bit Bitmap �f�[�^�ǂݍ���(24bit�p) */
bool
read08bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
throw()
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__(pct->width());

  UByte* linebuf;
  try
  {
    linebuf = new UByte[bufsize];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height()-1);
  int j;
  for (j=0; j<pct->height(); j++, pctbuf-=pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    palpaint__(pctbuf, linebuf, pct->width(), pal);
  }

  delete[] linebuf;
  return true;
}

/* 24Bit Bitmap �f�[�^�ǂݍ���(24bit�p) */
bool read24bit__(std::istream& is, polymnia::Picture* pct) throw()
{
  using namespace themis;
  using namespace polymnia;

  int w = pct->width();
  int o = pct->offset();

  int linesize = getBufSize__(w*3);

  UByte* imgbuf;
  try
  {
    imgbuf = new UByte[linesize*pct->height()];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  RgbColor* resbuf = pct->buffer();

  is.read((char*)imgbuf, linesize*pct->height());


  for (int j=(pct->height()-1)*o, l=0; j>=0; j-=o, l+=linesize)
    for (int i=0, k=0; i<w; i++, k+=3)
      resbuf[i+j] = RgbColor(imgbuf[l+k+2], imgbuf[l+k+1], imgbuf[l+k]);

  delete[] imgbuf;
  return true;
}

/* 32Bit Bitmap �f�[�^�ǂݍ���(32bit�p) */
bool read32bit__(std::istream& is, polymnia::Picture* pct) throw()
{
  using namespace themis;
  using namespace polymnia;

  int w = pct->width();
  int o = pct->offset();

  int linesize = getBufSize__(w * 4);

  UByte* imgbuf;
  try
  {
    imgbuf = new UByte[linesize*pct->height()];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  RgbColor* resbuf = pct->buffer();

  is.read((char*)imgbuf, linesize*pct->height());


  for (int j=(pct->height()-1)*o, l=0; j>=0; j-=o, l+=linesize)
    for (int i=0, k=0; i<w; i++, k+=4)
      resbuf[i+j] = RgbColor(imgbuf[l+k+2], imgbuf[l+k+1], imgbuf[l+k]);

  delete[] imgbuf;
  return true;
}


/* 1Bit Bitmap �f�[�^�ǂݍ���(8bit�p) */
bool read01bit__(std::istream& is, polymnia::PictureIndexed* pct) throw()
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width()+7)/8);
  UByte* linebuf;
  try
  {
    linebuf = new UByte[bufsize];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset() * (pct->height()-1);
  int j;
  for (j=0; j<pct->height(); j++, pctbuf-=pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    read01_oneline__(pctbuf, linebuf, pct->width());
  }

  delete[] linebuf;
  return true;
}


/* 4Bit Bitmap �f�[�^�ǂݍ���(8bit�p) */
bool read04bit__(std::istream& is, polymnia::PictureIndexed* pct) throw()
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width()+1)/2);
  UByte* linebuf;
  try
  {
    linebuf = new UByte[bufsize];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset()*(pct->height()-1);
  int j;
  for (j=0; j<pct->height(); j++, pctbuf-=pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    read04_oneline__(pctbuf, linebuf, pct->width());
  }

  delete[] linebuf;
  return true;
}

/* 8Bit Bitmap �f�[�^�ǂݍ���(8bit�p) */
bool read08bit__(std::istream& is, polymnia::PictureIndexed* pct) throw()
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  int bufsize = getBufSize__(pct->width());
  UByte* linebuf;
  try
  {
    linebuf = new UByte[bufsize];
  }
  catch(std::bad_alloc)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset() * (pct->height()-1);
  int j;
  for (j=0; j<pct->height(); j++, pctbuf-=pct->offset())
  {
    if (!is.read((char*)linebuf, bufsize))
      return false;

    memcpy(pctbuf, linebuf, pct->width());
  }

  delete[] linebuf;
  return true;
}




}//end of namespace



/*===========================
 *  DibLoader::load_()
 *  DIB�̓ǂݍ���
 */
polymnia::Picture* polymnia::DibLoader::load_(const char* path)
{
  using namespace std;

  Picture* q = nullptr;

  try
  {
    ifstream ifs(path, ios::in|ios::binary);
    if (!ifs)
      throw Error();

    if (!readHeader__(ifs))
      throw Error();

    Info_ info;
    if (!readInfo__(ifs, &info))
      throw Error();

    if (info.bit!=1 && info.bit!=4 && info.bit!=8 && info.bit!=24
        && info.bit!=32)
      throw Error();

    RgbColor pal[256];
    if (info.bit!=24 && info.bit!=32)
    {
      int np = info.npal;
      if (np==0)
        np = 1 << info.bit;
      if (!readPalette__(ifs, pal, np))
        throw Error();
    }

    q = Picture::create(info.w, info.h);
    if (!q)
      throw Error();

    switch(info.bit)
    {
    case 1:
      if (!read01bit__(ifs, q, pal))
        throw Error();
      break;

    case 4:
      if (!read04bit__(ifs, q, pal))
        throw Error();
      break;

    case 8:
      if (!read08bit__(ifs, q, pal))
        throw Error();
      break;

    case 24:
      if (!read24bit__(ifs, q))
        throw Error();
      break;

    case 32:
      if (!read32bit__(ifs, q))
        throw Error();
      break;

    default:
      throw Error();
    }

    return q;
  }
  catch(Error)
  {
    delete q;
    return nullptr;
  }
}


/*=================================
 *  IndexedDibLoader::load_()
 *  �p���b�gDIB�̓ǂݍ���
 */
polymnia::PictureIndexed*
polymnia::IndexedDibLoader::load_(const char* path)
{
  using namespace std;

  PictureIndexed* q = nullptr;
  try
  {
    ifstream ifs(path, ios::in|ios::binary);
    if (!ifs)
      throw Error();

    if (!readHeader__(ifs))
      throw Error();

    Info_ info;
    if (!readInfo__(ifs, &info))
      throw Error();


    if (info.bit!=1 && info.bit!=4 && info.bit!=8)
      throw Error();


    q = PictureIndexed::create(info.w, info.h);
    if (!q)
      throw Error();

    int np = info.npal;
    if (np==0)
      np = 1<<info.bit;
    if (!readPalette__(ifs, q->paletteBuffer(), np))
      throw Error();

    switch(info.bit)
    {
    case 1:
      if (!read01bit__(ifs, q))
        throw Error();
      break;

    case 4:
      if (!read04bit__(ifs, q))
        throw Error();
      break;

    case 8:
      if (!read08bit__(ifs,q))
        throw Error();
      break;

    default:
      throw Error();
    }

    return q;
  }
  catch(Error)
  {
    delete q;
    return nullptr;
  }
}


//eof
