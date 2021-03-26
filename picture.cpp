/*
 * Copyright 2001-2021 oZ/acy (名賀月晃嗣)
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
 * @file picture.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief Picture、PictureIndexedの生成處理及び複製處理
 * 
 * @date 2016.3.2   C++11對應(假)
 * @date 2018.12.27 catch(bad_alloc)をcatch(bad_alloc&)に修正
 * @date 2019.8.29  create系統の返却型をunique_ptrに變更
 */

#include <algorithm>
#include "picture.h"


/*========================================================
 *  Picture::create()
 *  幅, 高さを指定して Picture オブジェクトを生成
 *======================================================*/
std::unique_ptr<polymnia::Picture>
polymnia::Picture::create(unsigned w, unsigned h) noexcept
{
  try {
    return std::unique_ptr<Picture>(new Picture(w, h));
  }
  catch (std::bad_alloc&) {
    return nullptr;
  }
}


/*=================================
 *  Picture::clone()
 *  Picture の自己複製
 *===============================*/
std::unique_ptr<polymnia::Picture>
polymnia::Picture::clone() const noexcept
{
  auto res = create(w_, h_);
  if (res)
    std::copy(buf_, buf_ + h_ * offset_, res->buf_);
  return res;
}


/*==========================================================
 *  PictureIndexed::create()
 *  PictureIndexed オブジェクトを幅, 高さを指定して生成
 *========================================================*/
std::unique_ptr<polymnia::PictureIndexed>
polymnia::PictureIndexed::create(unsigned w, unsigned h) noexcept
{
  try {
    return std::unique_ptr<PictureIndexed>(new PictureIndexed(w, h));
  }
  catch(std::bad_alloc&) {
    return nullptr;
  }
}


/*====================================================
 *  PictureIndexed::clone()
 *  PictureIndexed オブジェクトの自己複製
 *==================================================*/
std::unique_ptr<polymnia::PictureIndexed>
polymnia::PictureIndexed::clone() const noexcept
{
  auto res = create(w_, h_);
  if (res) {
    std::copy(buf_, buf_ + h_ * offset_, res->buf_);
    std::copy(pal_, pal_ + 256, res->pal_);
  }
  return res;
}


/*=======================================================================
 *  PictureIndexed::duplicatePicture()
 *  PictureIndexed から Picture オブジェクトを複製 (但し色形式は変更)
 *=====================================================================*/
std::unique_ptr<polymnia::Picture>
polymnia::PictureIndexed::duplicatePicture() const noexcept
{
  auto pc = Picture::create(w_, h_);

  if (pc) {
    int area = h_ * offset_;
    for (int i = 0; i < area; i++)
      pc->buffer()[i] = pal_[buf_[i]];

    //上記は下記からの書き直し
    //for (int y=0; y<h_; y++)
    //  for (int x=0; x<w_; x++)
    //    pc->pixel(x, y) = pal_[buf_[x+y*offset_]];
  }
  return pc;
}


//eof
