/**************************************************************************
 *
 *  dibin.cpp
 *
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  DIB INput
 *  DIB形式画像入力用クラス実装
 *
 *  履歴
 *    2016.3.2    C++11對應
 *    2018.12.23  C++17對應
 *    2018.12.28  資源管理をunique_ptrに移行
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <new>
#include <memory>
#include "dibio.h"


namespace
{
/*----------------------------------------------
*  struct Info_
*  DIBファイル情報(の一部)
*---------------------------------------------*/
struct Info_
{
  int w;    //幅
  int h;    //高さ
  int bit;  //Bit per Pixel
  int npal; //パレット数
};


//////////////////////////////////////////////////
// ファイル先頭部の読み込み部分
//////////////////////////////////////////////////

/*==============================================
*  readHeader__()
*  DIBヘッダ読み込み 
*=============================================*/
bool readHeader__(std::istream& is)
{
  using namespace themis;

  UWord w;
  UDWord d;

  // マジックナンバー("BM")の読み込みと照合
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;
  if (w!=*(UWord*)("BM"))
    return false;

  // ファイルサイズ読み込み
  if (!is.read((char*)&d, sizeof(UDWord)))
    return false;

  // 予約済ワード(2つ)読み込み
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;
  if (!is.read((char*)&w, sizeof(UWord)))
    return false;

  // BitDataへのオフセット読み込み
  if (!is.read((char*)&d, sizeof(UDWord)))
    return false;

  return true;
}

/*========================================================
*  readInfo__()
*  BMP-Infoヘッダを讀み込む (struct Info に値を設定)
*=======================================================*/
bool readInfo__(std::istream& is, Info_* pi)
{
  using namespace themis;

  UWord w;
  UDWord u;
  SDWord d;

  // InfoHeader のサイズ
  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;

  // 畫像幅
  if (!is.read((char*)&d, sizeof(SDWord)))
    return false;
  pi->w = d;

  // 畫像高さ
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

  // パレット數
  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;
  pi->npal = u;

  if (!is.read((char*)&u, sizeof(UDWord)))
    return false;

  return true;
}

/*==============================================
*  readPalette__()
*  パレット読み込み
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

  if (!is.read((char*)quad, sizeof(UByte) * 4 * npal))
    return false;

  for (int i = 0; i < npal; i++)
    pal[i] = polymnia::RgbColor(quad[i].r, quad[i].g, quad[i].b);

  return true;
}

/*===================================================
*  getBufSize__()
*  Bitmap の 1line のバッファ長を求める
*==================================================*/
inline int getBufSize__(int l)
{
  return (l + 3) & ~3;
}


/*=================================================
*  パレット -> RGB 處理
*================================================*/
void
palpaint__(
  polymnia::RgbColor dbuf[], const themis::UByte sbuf[], int w,
  const polymnia::RgbColor pal[])
{
  for (int i = 0; i < w; i++)
    dbuf[i] = pal[sbuf[i]];
}

/*=============================================
*  read01_oneline__()
*  1bit bitmap データを1ライン読み込み
*============================================*/
void
read01_oneline__(themis::UByte dbuf[], themis::UByte sbuf[], int w)
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
*  4bit bitmap データを1ライン読み込み
*===============================================*/
void
read04_oneline__(themis::UByte dbuf[], themis::UByte sbuf[], int w)
{
  using namespace themis;

  for (int i = 0; i < w / 2; i++)
  {
    dbuf[i * 2 + 1] = (UByte)(sbuf[i] & 0x0F);
    dbuf[i * 2] = (UByte)((sbuf[i] >> 4) & 0x0F);
  }

  if (w & 1)
    dbuf[w - 1] = (UByte)(sbuf[w / 2] & 0x0F);
}


/*====================================================
*  read01bit__()
*  1Bit Bitmap データ読み込み(24bit用)
*===================================================*/
bool
read01bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width() + 7) / 8);

  std::unique_ptr<UByte[]> linebuf;
  std::unique_ptr<UByte[]> palbuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
    palbuf.reset(new UByte[pct->width()]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  for (int j = 0; j < pct->height(); j++, pctbuf += pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    read01_oneline__(palbuf.get(), linebuf.get(), pct->width());
    palpaint__(pctbuf, palbuf.get(), pct->width(), pal);
  }

  return true;
}


/*=============================================
*  read04bit__()
*  4Bit Bitmap データ読み込み(24bit用)
*============================================*/
bool
read04bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width()+1)/2);

  std::unique_ptr<UByte[]> linebuf;
  std::unique_ptr<UByte[]> palbuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
    palbuf.reset(new UByte[pct->width()]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height()-1);
  for (int j = 0; j<pct->height(); j++, pctbuf+=pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    read04_oneline__(palbuf.get(), linebuf.get(), pct->width());
    palpaint__(pctbuf, palbuf.get(), pct->width(), pal);
  }

  return true;
}


/* 8Bit Bitmap データ読み込み(24bit用) */
bool
read08bit__(
  std::istream& is, polymnia::Picture* pct, const polymnia::RgbColor pal[])
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__(pct->width());

  std::unique_ptr<UByte[]> linebuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  RgbColor* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  for (int j = 0; j<pct->height(); j++, pctbuf-=pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    palpaint__(pctbuf, linebuf.get(), pct->width(), pal);
  }

  return true;
}


/* 24Bit Bitmap データ読み込み(24bit用) */
bool read24bit__(std::istream& is, polymnia::Picture* pct)
{
  using namespace themis;
  using namespace polymnia;

  int w = pct->width();
  int o = pct->offset();

  int linesize = getBufSize__(w * 3);

  std::unique_ptr<UByte[]> imgbuf;
  try
  {
    imgbuf.reset(new UByte[linesize * pct->height()]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  RgbColor* resbuf = pct->buffer();

  is.read((char*)(imgbuf.get()), linesize * pct->height());


  for (int j = (pct->height()-1) * o, l = 0; j >= 0; j -= o, l += linesize)
    for (int i = 0, k = 0; i < w; i++, k += 3)
      resbuf[i + j] = RgbColor(imgbuf[l+k+2], imgbuf[l+k+1], imgbuf[l+k]);

  return true;
}


/* 32Bit Bitmap データ読み込み(32bit用) */
bool read32bit__(std::istream& is, polymnia::Picture* pct)
{
  using namespace themis;
  using namespace polymnia;

  int w = pct->width();
  int o = pct->offset();

  int linesize = getBufSize__(w * 4);

  std::unique_ptr<UByte[]> imgbuf;
  try
  {
    imgbuf.reset(new UByte[linesize*pct->height()]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  RgbColor* resbuf = pct->buffer();

  is.read((char*)(imgbuf.get()), linesize*pct->height());


  for (int j = (pct->height() - 1) * o, l = 0; j >= 0; j -= o, l += linesize)
    for (int i = 0, k = 0; i < w; i++, k += 4)
      resbuf[i + j]
       = RgbColor(imgbuf[l + k + 2], imgbuf[l + k + 1], imgbuf[l + k]);

  return true;
}


/* 1Bit Bitmap データ読み込み (Indexed Color用) */
bool read01bit__(std::istream& is, polymnia::PictureIndexed* pct)
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width() + 7) / 8);

  std::unique_ptr<UByte[]> linebuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  for (int j = 0; j < pct->height(); j++, pctbuf -= pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    read01_oneline__(pctbuf, linebuf.get(), pct->width());
  }

  return true;
}


/* 4Bit Bitmap データ読み込み(8bit用) */
bool read04bit__(std::istream& is, polymnia::PictureIndexed* pct)
{
  using namespace themis;
  using namespace polymnia;

  int bufsize = getBufSize__((pct->width()+1)/2);
  std::unique_ptr<UByte[]> linebuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  for (int j = 0; j < pct->height(); j++, pctbuf -= pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    read04_oneline__(pctbuf, linebuf.get(), pct->width());
  }

  return true;
}


/* 8Bit Bitmap データ読み込み(8bit用) */
bool read08bit__(std::istream& is, polymnia::PictureIndexed* pct)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  int bufsize = getBufSize__(pct->width());
  unique_ptr<UByte[]> linebuf;
  try
  {
    linebuf.reset(new UByte[bufsize]);
  }
  catch(std::bad_alloc&)
  {
    return false;
  }

  UByte* pctbuf = pct->buffer() + pct->offset() * (pct->height() - 1);
  for (int j=0; j < pct->height(); j++, pctbuf -= pct->offset())
  {
    if (!is.read((char*)(linebuf.get()), bufsize))
      return false;

    memcpy(pctbuf, linebuf.get(), pct->width());
  }

  return true;
}




}//end of namespace



/*===========================
 *  DibLoader::load()
 *  DIBの読み込み
 */
polymnia::Picture* polymnia::DibLoader::load(const std::filesystem::path& path)
{
  using namespace std;


  ifstream ifs(path, ios::in|ios::binary);
  if (!ifs)
    return nullptr;

  if (!readHeader__(ifs))
    return nullptr;

  Info_ info;
  if (!readInfo__(ifs, &info))
    return nullptr;

  if (info.bit!=1 && info.bit!=4 && info.bit!=8 && info.bit!=24
      && info.bit!=32)
    return nullptr;

  RgbColor pal[256];
  if (info.bit!=24 && info.bit!=32)
  {
    int np = info.npal;
    if (np==0)
      np = 1 << info.bit;
    if (!readPalette__(ifs, pal, np))
      return nullptr;
  }

  unique_ptr<Picture> q(Picture::create(info.w, info.h));
  if (!q)
    return nullptr;

  switch(info.bit)
  {
  case 1:
    if (!read01bit__(ifs, q.get(), pal))
      return nullptr;
    break;

  case 4:
    if (!read04bit__(ifs, q.get(), pal))
      return nullptr;
    break;

  case 8:
    if (!read08bit__(ifs, q.get(), pal))
      return nullptr;
    break;

  case 24:
    if (!read24bit__(ifs, q.get()))
      return nullptr;
    break;

  case 32:
    if (!read32bit__(ifs, q.get()))
      return nullptr;
    break;

  default:
      return nullptr;
  }

  return q.release();
}


/*=================================
 *  IndexedDibLoader::load()
 *  パレットDIBの読み込み
 */
polymnia::PictureIndexed*
polymnia::IndexedDibLoader::load(const std::filesystem::path& path)
{
  using namespace std;

  ifstream ifs(path, ios::in|ios::binary);
  if (!ifs)
    return nullptr;

  if (!readHeader__(ifs))
    return nullptr;

  Info_ info;
  if (!readInfo__(ifs, &info))
    return nullptr;

  if (info.bit!=1 && info.bit!=4 && info.bit!=8)
    return nullptr;


  unique_ptr<PictureIndexed> q(PictureIndexed::create(info.w, info.h));
  if (!q)
    return nullptr;

  int np = info.npal;
  if (np == 0)
    np = 1 << info.bit;
  if (!readPalette__(ifs, q->paletteBuffer(), np))
    return nullptr;

  switch(info.bit)
  {
  case 1:
    if (!read01bit__(ifs, q.get()))
      return nullptr;
    break;

  case 4:
    if (!read04bit__(ifs, q.get()))
      return nullptr;
    break;

  case 8:
    if (!read08bit__(ifs, q.get()))
      return nullptr;
    break;

  default:
    return nullptr;
  }

  return q.release();
}


//eof
