/***************************************************************
 *
 *  main.cpp
 *  by oZ/acy
 *  (c) 2003-2019 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  畫像擴大縮小プログラム本體
 *
 *  履歴
 *    R1.7.16 擴大縮小函數のPictureメンバ化に對應
 */

#include <iostream>
#include <cstdlib>
#include "resizer.h"


/*===========================================
 *  main()
 */
int main(int argc, char** argv)
{
  using namespace polymnia;


  bool valid;
  bool isP;  // true : -p power / false : -l width height
  int w, h;
  double r;
  std::string src, dst;


  valid = false;
  if (argc > 2)
  {
    std::string opt = argv[1];
    if (opt == "-p" && argc == 5)
    {
      r = std::atof(argv[2]);
      if (r <= 0)
      {
        std::cerr << "bad magnifying power.\n";
        return 1;
      }
      isP = true;
      src = argv[3];
      dst = argv[4];
      valid = true;
    }
    else if (opt == "-l" && argc == 6)
    {
      w = std::atoi(argv[2]);
      h = std::atoi(argv[3]);
      if (w <= 0 || h <= 0)
      {
        std::cerr << "bad width or height.\n";
        return 1;
      }
      isP = false;
      src = argv[4];
      dst = argv[5];
      valid = true;
    }
  }

  if (!valid)
  {
    std::cerr << "usage: resizer -p power infile outfile\n";
    std::cerr << "       resizer -l width height infile outfile\n";
    return 1;
  }

  Picture* pict = loadImage(src);
  if (!pict)
  {
    std::cerr << "cannot open file " << src << ".\n";
    return 1;
  }

  bool issmall;
  if (isP)
  {
    w = (int)(pict->width() * r);
    h = (int)(pict->height() * r);
    issmall = r < 1.0;
  }
  else
  {
    issmall = w < pict->width() && h < pict->height();
  }

  Picture* pict2;
  if (issmall)
    pict2 = pict->createReducedPicture(w, h);
  else
    pict2 = pict->createMagnifiedPicture(w, h);

  if (pict2)
    saveImage(dst, pict2);
  else
  {
    std::cerr << "cannot resize image.\n";
    return 1;
  }

  delete pict;
  delete pict2;
  return 0;
}


//eof
