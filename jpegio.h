/**********************************************************************//**
 *
 *  @file jpegio.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief JPEG畫像入出力クラス (libjpegのラッパー)
 *
 *  @date 2018.12.23 C++17對應
 *//*
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 */
#ifndef INC_POLYMNIA_JPEGIO_H__
#define INC_POLYMNIA_JPEGIO_H__

#include "picture.h"
#include "pictio.h"

namespace polymnia
{
  class JpegLoader;
  class JpegSaver;
}


/*--------------------------------------------*//**
 *  @brief JPEG形式畫像の讀み込み用クラス
 */
class polymnia::JpegLoader : public polymnia::PictLoader<polymnia::Picture>
{
public:
  /// @brief 構築子
  JpegLoader() {}
  /// @brief 解體子
  ~JpegLoader() {}

  /// @brief 讀み込み
  ///
  /// 指定されたファイルから畫像を讀み込む。
  /// @param path ファイルパス
  /// @return
  ///  讀み込んだ畫像を格納した畫像バッファオブジェクトを返す。
  ///  ファイル形式が異なるなど讀み込めない場合にはnullptrを返す。
  polymnia::Picture* load(const std::filesystem::path& path) override;
};


/*------------------------------------------*//**
 *  @brief JPEG形式畫像の保存用クラス
 */
class polymnia::JpegSaver : public polymnia::PictSaver<polymnia::Picture>
{
private:
  int quality;  ///< クオリティ
  bool prog;    ///< プログレッシブ形式で保存するかどうか

public:
  /// @brief 構築子
  /// @param q クオリティ デフォルト値は75
  /// @param g プログレッシブ形式で保存するかどうか デフォルト値はfalse
  JpegSaver(int q=75, bool g =false) : quality(q), prog(g) {}

  /// @brief 解體子
  ~JpegSaver() {}

  /// @brief クオリティの設定
  ///
  /// クオリティを0〜100の間で設定する。
  void setQuality(int q) { quality = q; }

  /// @brief プログレッシブ形式で保存するか否かの設定
  ///
  /// プログレッシブ形式で保存する場合はtrue、
  /// さもなくばfalseを設定する。
  void setProgressive(bool g) { prog = g; }

  /// @brief 保存
  bool
    save(
    const polymnia::Picture* p, const std::filesystem::path& path) override;
};


#endif // INC_POLYMNIA_JPEGIO_H__
