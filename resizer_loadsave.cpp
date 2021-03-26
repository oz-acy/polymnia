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
 * @file resizer_loadsave.cpp
 * @author oZ/acy
 * @brief 畫像擴大縮小プログラムのファイル入出力關聯の處理
 *
 * @date 2019.8.24 修正
 *
 */

#include <cctype>
#include "dibio.h"
#include "pngio.h"
#include "jpegio.h"
#include "resizer.h"


/**************************************************************
 *  文字列操作關聯
 *  いづれライブラリ化すへし
 *************************************************************/

/*================================================
 *  makeLower()
 *  大文字を小文字に變換した文字列を生成
 *==============================================*/
std::string makeLower(const std::string& str)
{
  std::string res = str;
  std::string::iterator i;
  for (i=res.begin(); i!=res.end(); i++)
    *i = (char)std::tolower(*i);
  return res;
}


/*=============================================
 *  toLower()
 *  文字列中の大文字を小文字で置換
 *===========================================*/
void toLower(std::string str)
{
  std::string::iterator i;
  for (i=str.begin(); i!=str.end(); i++)
    *i = (char)std::tolower(*i);
}


/*================================================
 *  makeUpper()
 *  小文字を大文字に變換した文字列を生成
 *==============================================*/
std::string makeUpper(const std::string& str)
{
  std::string res = str;
  std::string::iterator i;
  for (i=res.begin(); i!=res.end(); i++)
    *i = (char)std::toupper(*i);
  return res;
}


/*=============================================
 *  toUpper()
 *  文字列中の小文字を大文字で置換
 *===========================================*/
void toUpper(std::string str)
{
  std::string::iterator i;
  for (i=str.begin(); i!=str.end(); i++)
    *i = (char)std::toupper(*i);
}


/*****************************************************
 *  ファイル操作補助
 *  そのうちライブラリ化すへきかも
 ****************************************************/

/*===============================================
 *  getFileExt()
 *  ファイル(パス)名から擴張子を取り出す
 *  取り出した擴張子は全部小文字
 *=============================================*/
std::string getFileExt(const std::string& file)
{
  std::string::size_type n = file.rfind('.');
  if (n!=std::string::npos)
    return makeLower(file.substr(n+1, file.size()-n-1));
  else
    return std::string();
}



/*****************************************
 *  業務本體
 ****************************************/

/*============================================
 *  EIViewer::loadImage()
 *  畫像ファイルを擴張子に應じてロードする
 *==========================================*/
std::unique_ptr<polymnia::Picture> loadImage(const std::string& file)
{
  using namespace polymnia;

  std::unique_ptr<Picture> pict;
  std::string ext = getFileExt(file);

  if (ext=="bmp") {
    DibLoader bload;
    pict = bload.load(file);
  }
  else if (ext=="png") {
    PngLoader pload;
    pict = pload.load(file);
  }
  else if (ext=="jpg" || ext=="jpeg") {
    JpegLoader jload;
    pict = jload.load(file);
  }
  //else
  //  pict = nullptr;

  return pict;
}


/*========================================================
 *  saveImage()
 *  畫像を擴張子に應じた形式で保存する
 *======================================================*/
void saveImage(const std::string& file, const polymnia::Picture* pict)
{
  using namespace polymnia;

  std::string ext = getFileExt(file);

  if (ext=="png") {
    PngSaver psave;
    psave.save(pict, file);
  }
  else if (ext=="jpg" || ext=="jpeg") {
    JpegSaver jsave;
    jsave.save(pict, file);
  }
  else { // if (ext=="bmp") {
    DibSaver bsave;
    bsave.save(pict, file);
  }
}


//eof
