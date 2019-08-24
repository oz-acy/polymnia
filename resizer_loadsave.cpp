/***************************************************************
 *
 *  resizer_loadsave.cpp
 *  by oZ/acy
 *  (c) 2003-2019 oZ/acy, ALL RIGHTS RESERVED.
 *
 *  畫像擴大縮小プログラム ファイル讀み込み・保存關聯
 *
 *  last update : 2019.8.24
 *
 **************************************************************/

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
polymnia::Picture* loadImage(const std::string& file)
{
  using namespace polymnia;

  Picture* pict;
  std::string ext = getFileExt(file);

  if (ext=="bmp")
  {
    DibLoader bload;
    pict = bload.load(file);
  }
  else if (ext=="png")
  {
    PngLoader pload;
    pict = pload.load(file);
  }
  else if (ext=="jpg" || ext=="jpeg")
  {
    JpegLoader jload;
    pict = jload.load(file);
  }
  else
    pict = nullptr;

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

  if (ext=="png")
  {
    PngSaver psave;
    psave.save(pict, file);
  }
  else if (ext=="jpg" || ext=="jpeg")
  {
    JpegSaver jsave;
    jsave.save(pict, file);
  }
  else //if (ext=="bmp")
  {
    DibSaver bsave;
    bsave.save(pict, file);
  }
}


//eof
