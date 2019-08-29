/****************************************************************
 *
 *  resizer.h
 *  by oZ/acy (名賀月晃嗣)
 *
 *  畫像擴大縮小プログラム ヘッダ
 *
 *  last update: 2019.8.29
 *
 */
#ifndef INCLUDE_GUARD_RESIZER_H
#define INCLUDE_GUARD_RESIZER_H

#include "picture.h"

std::unique_ptr<polymnia::Picture> loadImage(const std::string& file);
void saveImage(const std::string& file, const polymnia::Picture* pict);


#endif // INCLUDE_GUARD_RESIZER_H
