/**************************************************************************
 *
 *  pngout.cpp
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  PNG �`���o�̓N���X
 *
 *  last update: 2018.12.23
 *
 */
#include <cstdio>
#include <iostream>
#include <fstream>

extern "C" {
#include <png.h>
}

#include "pngio.h"


namespace
{

/*________________________________________
 *
 *  ofstream �ɛ����鏑������
 *
 *  H23.4.24 update for libpng-1.5.x
 */
void pngWrite__(png_structp png_ptr, png_bytep data, png_size_t length)
{
  using namespace std;
  ofstream* pofs = reinterpret_cast<ofstream*>(png_get_io_ptr(png_ptr));
  pofs->write(reinterpret_cast<char*>(data), length);

  //  H23.4.24 comment out for libpng-1.5.x
  //((std::ofstream*)(png_ptr->io_ptr))->write((char*)data,length);
}


/* flush���� (�A���_�~�[) */
void pngFlushDammy__(png_structp)
{
  /* none */
}


/* �������݂̏����|�� */
bool pngWriteInit__(
  const std::filesystem::path& path, std::ofstream& ofs, png_structp& png_ptr,
  png_infop& info_ptr)
{
  ofs.open(path, std::ios::out|std::ios::binary);
  if (!ofs)
    return false;

  png_ptr = png_create_write_struct(
              PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr)
    return false;

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
  {
    png_destroy_write_struct(&png_ptr, nullptr);
    return false;
  }

  png_set_write_fn(png_ptr, (void *)&ofs, pngWrite__, pngFlushDammy__);

  return true;
}


}//end of namespace NONAME


bool
polymnia::PngSaver::save(
  const polymnia::Picture* pct, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngWriteInit__(path, ofs, png_ptr, info_ptr))
    return false;

  int bits = 8;
  int type = PNG_COLOR_TYPE_RGB;
  int i_val = interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
  int ww = pct->width();
  int hh = pct->height();

  png_set_IHDR(
    png_ptr, info_ptr, ww, hh, bits, type, i_val, PNG_COMPRESSION_TYPE_BASE,
    PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  // �C���^�[���X������ꍇ�̌J��Ԃ��d�ɂ��擾 (���Ȃ��ꍇ��1�Œ�)
  int n_pass;
  if (interlace)
    n_pass = png_set_interlace_handling(png_ptr);
  else
    n_pass = 1;


  const RgbColor* buf = pct->buffer();
  int o = pct->offset();
  for (int pass=0; pass < n_pass; pass++)
    for (int j=0, y=0; y < hh; y++, j+=o)
    {
      png_bytep pp = (png_bytep)&buf[j];
      png_write_rows(png_ptr, &pp, 1);
    }

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  return true;
}


bool
polymnia::IndexedPngSaver::save(
  const polymnia::PictureIndexed* pct, const std::filesystem::path& path)
{
  using namespace std;

  ofstream ofs;
  png_structp png_ptr;
  png_infop info_ptr;

  if (!pngWriteInit__(path, ofs, png_ptr, info_ptr))
    return false;

  int bits=8;
  int type = PNG_COLOR_TYPE_PALETTE;
  int i_val = interlace ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
  int ww = pct->width();
  int hh = pct->height();
  png_set_IHDR(
    png_ptr, info_ptr, ww, hh, bits, type, i_val, PNG_COMPRESSION_TYPE_BASE,
    PNG_FILTER_TYPE_BASE);


  // �p���b�g�֌W
  png_colorp palette = new png_color[256];
  for (int i=0; i<256; i++)
  {
    palette[i].red = pct->palette(i).r;
    palette[i].green = pct->palette(i).g;
    palette[i].blue = pct->palette(i).b;
  }
  png_set_PLTE(png_ptr, info_ptr, palette, 256);

  // ���ߏ���
  png_bytep transbf = new png_byte[256];
  if (trans)
  {
    for (int i=0; i < 256; i++)
      if (i == paltp)
        transbf[i] = 0;
      else
        transbf[i] = 0xff;

    png_set_tRNS(png_ptr, info_ptr, transbf, 256, nullptr);
  }

  png_write_info(png_ptr,info_ptr);


  // �C���^�[���X������ꍇ�̌J��Ԃ��d�ɂ��擾 (���Ȃ��ꍇ��1�Œ�)
  int n_pass;
  if(interlace)
    n_pass = png_set_interlace_handling(png_ptr);
  else
    n_pass = 1;


  const themis::UByte* buf = pct->buffer();
  int o = pct->offset();
  for (int pass = 0; pass < n_pass; pass++)
    for (int j=0, y=0; y < hh; y++, j+=o)
    {
      png_bytep pp = (png_bytep)&buf[j];
      png_write_rows(png_ptr,&pp,1);
    }

  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  delete[] palette;
  delete[] transbf;
  return true;
}


//eof
