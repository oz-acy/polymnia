/****************************************************************
*
*  resizer.h
*  by oZ/acy
*  (C) 2003-2019 oZ/acy, ALL RIGHTS RESERVED.
*
*  畫像擴大縮小プログラム ヘッダ
*
*  last update: 2019.8.24
*
****************************************************************/
#ifndef INCLUDE_GUARD_RESIZER_H
#define INCLUDE_GUARD_RESIZER_H

#include "picture.h"

polymnia::Picture* loadImage(const std::string& file);
void saveImage(const std::string& file, const polymnia::Picture* pict);


#endif // INCLUDE_GUARD_RESIZER_H
