/**************************************************************************
 *
 *  dibout.cpp
 *
 *  (C) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  DIB OUTput
 *  DIB形式画像出力用クラス実装
 *
 *  履歴
 *    2018.12.23 C++17對應
 */

#include<iostream>
#include<fstream>
#include<cstring>
#include "dibio.h"


namespace
{

// DIBファイル情報(の一部)
struct Info_
{
  int w;    //幅
  int h;    //高さ
  int bit;  //Bit per Pixel
  int npal; //パレット数
};


/* DIBヘッダ書き出し */
bool writeHeader__(std::ostream& os, int bitsize, int npal)
{
  using namespace themis;

  UDWord dword;
  UWord word;

  // マジックナンバー書き出し
  word = *(UWord*)("BM");
  os.write((char*)(&word), sizeof(UWord));

  // ファイルサイズ書き出し
  dword = 54 + bitsize + 4 * npal;
  os.write((char*)(&dword), sizeof(UDWord));

  // 予約済ワード(2つ)を0で書き出し
  word = 0;
  os.write((char*)(&word), sizeof(UWord));
  os.write((char*)(&word), sizeof(UWord));

  // BitDataへのオフセット書き出し
  dword = 54 + 4 * npal;
  os.write((char*)(&dword), sizeof(UDWord));

  return true;
}


/* BMP-Infoヘッダ書き出し */
bool writeInfo__(std::ostream& os, int w, int h, themis::UWord bits, int npal)
{
  using namespace themis;

  UWord r; SDWord d; UDWord u;

  u = 5*sizeof(UDWord) + 4*sizeof(SDWord) + 2*sizeof(UWord);
  os.write((char*)&u, sizeof(UDWord));

  d = w;
  os.write((char*)&d, sizeof(SDWord));
  d = h;
  os.write((char*)&d, sizeof(SDWord));
  r = 1;
  os.write((char*)&r, sizeof(UWord));
  r = bits;
  os.write((char*)&r, sizeof(UWord));

  u = 0;
  os.write((char*)&u, sizeof(UDWord));
  os.write((char*)&u, sizeof(UDWord));
  d = 0;
  os.write((char*)&d, sizeof(SDWord));
  os.write((char*)&d, sizeof(SDWord));

  u = npal;
  os.write((char*)&u, sizeof(UDWord));
  u = 0;
  os.write((char*)&u, sizeof(UDWord));

  return true;
}


/* 8bitパレット書き出し */
bool writePalette__(std::ostream& os, const polymnia::RgbColor pal[], int np)
{
  using namespace themis;

  struct
  {
    UByte b;
    UByte g;
    UByte r;
    UByte v;
  } quad[256];

  for (int i = 0; i < np; i++)
  {
    quad[i].r = pal[i].r;
    quad[i].g = pal[i].g;
    quad[i].b = pal[i].b;
    quad[i].v = 0;
  }

  os.write((char*)quad, sizeof(UByte) * 4 * np);

  return true;
}


/* 24bit Bitmapデータ書き出し */
bool writeBits__(std::ostream& os, const polymnia::Picture* pct, int bufsize)
{
  using namespace themis;
  using namespace std;

  int w = pct->width();
  UByte* linebuf = new UByte[bufsize];

  memset(linebuf, 0, bufsize);

  for (int j = pct->height() - 1; j>=0; j--)
  {
    int p = w * j;
    for (int i=0; i<w; i++)
    {
      const polymnia::RgbColor& c = pct->buffer()[p+i];
      linebuf[3*i] = c.b;
      linebuf[3*i+1] = c.g;
      linebuf[3*i+2] = c.r;
    }
    os.write((char*)linebuf, bufsize);
  }

  delete[] linebuf;
  return true;
}


/* 8bit Bitmap 書き出し */
bool writeBits__(
  std::ostream& os, const polymnia::PictureIndexed* pct, int bufsize)
{
  using namespace themis;
  using namespace std;

  int w = pct->width();
  UByte* linebuf = new UByte[bufsize];

  memset(linebuf, 0, bufsize);

  for (int j = pct->height() - 1; j>=0; j--)
  {
    memcpy(linebuf, pct->buffer()+w*j, w);
    os.write((char*)linebuf, bufsize);
  }

  delete[] linebuf;
  return true;
}




}//end of namespace



/*==============================
 *  DibSaver::save_()
 *  24bitDIBの保存
 */
bool
polymnia::DibSaver::save(
  const polymnia::Picture* p, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs(path, ios::out|ios::binary);
  if (!ofs)
    return false;

  int bufsize = p->width() * 3;
  bufsize = (bufsize % 4) ? 4 * (bufsize/4 + 1) : bufsize;

  int mapsize = bufsize * p->height();

  writeHeader__(ofs, mapsize, 0);
  writeInfo__(ofs, p->width(), p->height(), 24, 0);
  writeBits__(ofs, p, bufsize);

  return true;
}


/*=============================
 *  IndexedDibSaver::save_()
 *  パレットDIBの保存
 */
bool
polymnia::IndexedDibSaver::save(
  const polymnia::PictureIndexed* p, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs(path, ios::out|ios::binary);
  if (!ofs)
    return false;

  int bufsize = p->width();
  bufsize = (bufsize % 4) ? 4 * (bufsize/4 + 1) : bufsize;

  int mapsize = bufsize * p->height();

  writeHeader__(ofs, mapsize, 256);
  writeInfo__(ofs, p->width(), p->height(), 8, 0);
  writePalette__(ofs, p->paletteBuffer(), 256);
  writeBits__(ofs, p, bufsize);

  return true;
}


//eof
