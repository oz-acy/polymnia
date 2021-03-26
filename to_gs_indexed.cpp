/*
 * Copyright 2008-2021 oZ/acy (名賀月晃嗣)
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
 * @file to_gs_indexed.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief グレイスケール化したPictureIndexedの生成
 *
 * @date 2008.4.20 作成
 * @date 2008.10.3 修正
 * @date 2011.9.8 修正
 * @date 2016.2.26 ファイル名變更
 * @date 2016.3.2 C++11對應
 * @date 2018.12.28 createPictureGrayScaleIndexed()をPictureのメンバ函數に編入
 * @date 2019.8.29 返却型をunique_ptrに變更
 * @date 2021.3.25 themis/inttype.hの非推奬化に對應
 */
#include "picture.h"


/*================================================
 *  グレイスケール + Indexed化
 */
std::unique_ptr<polymnia::PictureIndexed>
polymnia::Picture::createPictureGrayScaleIndexed() const noexcept
{
  using namespace themis;

  /* PictureIndexed生成 */
  auto pc = PictureIndexed::create(w_, h_);
  if (!pc)
    return nullptr;

  /* パレット作成 */
  for (int i = 0; i < 256; i++)
    pc->palette(i)
      = RgbColor((std::uint8_t)i, (std::uint8_t)i, (std::uint8_t)i);

  /* グレイスケール化 */
  for (int y = 0; y < h_; y++) {
    for (int x = 0; x < w_; x++) {
      RgbColor col = pixel(x, y);
      std::uint8_t l = col.r * 0.2990 + col.g * 0.5870 + col.b * 0.1140;
      pc->pixel(x, y) = l;
    }
  }

  return pc;
}




//eof
