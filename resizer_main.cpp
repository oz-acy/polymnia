/*
 * Copyright 2003-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * @file main.cpp
 * @author oZ/acy
 * @brief 畫像擴大縮小プログラム本體
 *
 * @date R1.7.16 擴大縮小函數のPictureメンバ化に對應
 *
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
  if (argc > 2) {
    std::string opt = argv[1];
    if (opt == "-p" && argc == 5) {
      r = std::atof(argv[2]);
      if (r <= 0) {
        std::cerr << "bad magnifying power.\n";
        return 1;
      }
      isP = true;
      src = argv[3];
      dst = argv[4];
      valid = true;
    }
    else if (opt == "-l" && argc == 6) {
      w = std::atoi(argv[2]);
      h = std::atoi(argv[3]);
      if (w <= 0 || h <= 0) {
        std::cerr << "bad width or height.\n";
        return 1;
      }
      isP = false;
      src = argv[4];
      dst = argv[5];
      valid = true;
    }
  }

  if (!valid) {
    std::cerr << "usage: resizer -p power infile outfile\n";
    std::cerr << "       resizer -l width height infile outfile\n";
    return 1;
  }

  std::unique_ptr<Picture> pict = loadImage(src);
  if (!pict) {
    std::cerr << "cannot open file " << src << ".\n";
    return 1;
  }

  bool issmall;
  if (isP) {
    w = (int)(pict->width() * r);
    h = (int)(pict->height() * r);
    issmall = r < 1.0;
  }
  else {
    issmall = w < pict->width() && h < pict->height();
  }

  std::unique_ptr<Picture> pict2;
  if (issmall)
    pict2 = pict->createReducedPicture(w, h);
  else
    pict2 = pict->createMagnifiedPicture(w, h);

  if (pict2)
    saveImage(dst, pict2.get());
  else {
    std::cerr << "cannot resize image.\n";
    return 1;
  }


  return 0;
}


//eof
