/*
 * Copyright 2007-2021 oZ/acy (名賀月晃嗣)
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
 *
 *  @file tograyscale24.cpp
 *  @author oZ/acy
 *  @brief Pictureの色變換ルーチン
 *
 *  @date 2016.2.26 ファイル名變更
 *  @date 2016.3.2 C++11對應
 *  @date 2018.12.28 convertToGrayScaledをPictureのメンバ函數に編入
 *  @date 2021.3.25 themis/inttype.hの非推奬化に對應
 */

#include "picture.h"


/*================================================
 *  convertToGrayScaled()
 *  グレイスケール化
 */
void polymnia::Picture::convertToGrayScaled() noexcept
{
  for (int i = 0; i < h_ * offset_; i++) {
    std::uint8_t l
      = buf_[i].r * 0.2990 + buf_[i].g * 0.5870 + buf_[i].b * 0.1140;
    buf_[i] = RgbColor(l, l, l);
  }
}




//eof
