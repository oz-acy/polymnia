/**********************************************************************//**
 *
 *  @file picture.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief RGB24bit色とRGB256パレットカラーの畫像バッファクラス
 * 
 *  @date 2016.3.2  C++11對應(假)
 *  @date 2018.12.28 グレイスケール化、擴大、縮小函數をメンバ函數化
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.29 createなどの返却型をunique_ptrに變更
 */
#ifndef INCLUDE_GUARD_POLYMNIA_PICTURE_H
#define INCLUDE_GUARD_POLYMNIA_PICTURE_H

#include <string>
#include <cstring>
#include <memory>
#include "ibuf.h"
#include "rgb.h"


namespace polymnia
{
  class Picture;
  class PictureIndexed;
}


/**
 *  @brief RGB24bitカラー畫像バッファ
 */
class polymnia::Picture : public polymnia::ImageBuffer<polymnia::RgbColor>
{
protected:
  /// @brief 構築子
  /// @param w 幅
  /// @param h 高さ
  Picture(unsigned w, unsigned h)
    : polymnia::ImageBuffer<polymnia::RgbColor>(w, h, w)
    { buf_ = new polymnia::RgbColor[h_ * offset_]; }

public:
  /// @brief 解體子
  ~Picture() { delete[] buf_; }

  /// 畫像バッファを生成する。
  static std::unique_ptr<Picture> create(unsigned w, unsigned h) noexcept;

  /// 複製する。
  std::unique_ptr<Picture> clone() const noexcept;

  /// 256色に減色したPictureIndexedオブジェクトを生成する。
  std::unique_ptr<PictureIndexed> duplicatePictureIndexed() const noexcept;

  /// グレイスケール化する。
  void convertToGrayScaled() noexcept;

  /// 内容をグレイスケール化したPictureIndexedオブジェクトを生成する。
  std::unique_ptr<polymnia::PictureIndexed>
  createPictureGrayScaleIndexed() const noexcept;


  /// 縮小したPictureを生成する。
  std::unique_ptr<Picture> createReducedPicture(int w, int h) const noexcept;

  /// 擴大したPictureを生成する。
  std::unique_ptr<Picture> createMagnifiedPicture(int w, int h) const noexcept;
};




/**
 * @brief RGB24bit256色パレットカラー畫像バッファ
 */
class polymnia::PictureIndexed : public polymnia::ImageBuffer<themis::UByte>
{
protected:
  polymnia::RgbColor pal_[256]; ///< パレット

  /// @brief 構築子
  /// @param w 幅
  /// @param h 高さ
  PictureIndexed(unsigned w, unsigned h)
    : polymnia::ImageBuffer<themis::UByte>(w, h, w)
    { buf_ = new themis::UByte[h_ * offset_]; }


public:
  /// 解體子
  ~PictureIndexed() { delete[] buf_; }

  /// 畫像バッファを生成する。
  static
  std::unique_ptr<PictureIndexed> create(unsigned w, unsigned h) noexcept;

  /// 複製する。
  std::unique_ptr<PictureIndexed> clone() const noexcept;


  // パレットへのアクセス
  /// パレットを參照する。
  /// @param id パレットID
  /// @return IDで指定されるパレットへの參照
  polymnia::RgbColor& palette(int id) noexcept { return pal_[id]; }
  /// パレットを參照する。
  /// @param id パレットID
  /// @return IDで指定されるパレットへの參照
  const polymnia::RgbColor& palette(int id) const noexcept { return pal_[id]; }
  /// パレットのバッファへのポインタを得る。
  polymnia::RgbColor* paletteBuffer() noexcept { return pal_; }
  /// パレットのバッファへのポインタを得る。
  const polymnia::RgbColor* paletteBuffer() const noexcept { return pal_; }

  /// 同内容のPictureを生成する。
  std::unique_ptr<Picture> duplicatePicture() const noexcept;
};



#endif // INCLUDE_GUARD_POLYMNIA_PICTURE_H
