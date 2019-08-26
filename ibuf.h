/**********************************************************************//**
 *
 *  @file ibuf.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief 畫像バッファのクラステンプレート
 *
 *  @date 2016.3.2  C++11/14對應(假)
 *  @date 2018.12.21  C++17對應(假)
 *  @date 2019.8.16 インクルードガードの識別子を豫約されてゐないものに修正
 *  @date 2019.8.26 boost::noncopyableをthemis::Noncopyable<>に差し替へ
 */
#ifndef INCLUDE_GUARD_POLYMNIA_IMAGEBUFFER_H
#define INCLUDE_GUARD_POLYMNIA_IMAGEBUFFER_H

#include <themis/exception.h>
#include <themis/noncopyable.h>


namespace polymnia
{

template<class C_> class ImageBuffer;

/**
 * @brief 點を表す構造體
 */
struct Point
{
  int x; ///< x座標
  int y; ///< y座標

  Point() = default;
  constexpr Point(int xx, int yy) : x(xx), y(yy) {}
};


/**
 *  @brief 長方形を表す構造體
 */
struct Rect
{
  int x1; ///< x座標1
  int y1; ///< y座標1
  int x2; ///< x座標2
  int y2; ///< y座標2

  Rect() = default;
  constexpr Rect(int x, int y, int xx, int yy)
    : x1(x), y1(y), x2(xx), y2(yy)
    {}
};


/**
 * @brief 範圍逸脱例外
 */
class RangeOverException : public themis::EmptyException
{
public:
  const char* what() const noexcept
  {
    static const char rstr[] = "polymnia::RangeOverException";
    return rstr;
  }
};

}//end of namespace polymnia




/*-----------------------------------------------------*//**
 *  @brief 畫像バッファ基底クラステンプレート
 */
template<class C_>
class polymnia::ImageBuffer : themis::Noncopyable<polymnia::ImageBuffer<C_>>
{
public:
  typedef C_ ColorType;

  //-------------------------------------
  //  class Effecter
  //  畫像効果追加用の抽象基底クラス
  //-------------------------------------
  //class Effecter
  //{
  //public:
  //  Effecter() {}
  //  virtual ~Effecter() {}
  //
  //  virtual void operator() (
  //    C_* buf, const C_* src, int w, int h, int offset) const =0;
  //};

//private:
//  typedef polymnia::ImageBuffer<C_>& Ref_;
//  typedef const polymnia::ImageBuffer<C_>& CR_;
//  typedef const polymnia::ImageBuffer<C_>* CP_;


protected:
  C_* buf_;  ///< 畫像バッファ
  int w_;  ///< 幅
  int h_;  ///< 高さ
  int offset_; ///< オフセット 水平方向の1ラインのバッファサイズ

  /// @brief 構築子
  /// @param w 幅
  /// @param h 高さ
  /// @param o オフセット
  ImageBuffer(int w, int h, int o) : buf_(nullptr), w_(w), h_(h), offset_(o) {}

public:
  /// 解體子
  virtual ~ImageBuffer() {}

  /// @brief 幅を取得
  int width() const noexcept { return w_; }
  /// @brief 高さを取得
  int height() const noexcept { return h_; }
  /// @brief オフセットを取得
  int offset() const noexcept { return offset_; }

  /// @brief バッファの先頭アドレスを取得
  C_* buffer() noexcept { return buf_; }
  /// @brief バッファの先頭アドレスを取得
  const C_* buffer() const noexcept { return buf_; }

  /// @brief ピクセル(x, y)を參照
  C_& pixel(int x, int y) noexcept { return buf_[x + y * offset_]; }
  /// @brief ピクセル(x, y)を參照
  const C_& pixel(int x, int y) const noexcept { return buf_[x + y * offset_]; }

  /// @brief ピクセル(x, y)を參照
  ///
  /// pixel()は(x, y)がバッファの範圍に含まれるか確認しないが、
  /// at()は範圍を確認し、範圍外の場合はRangeOverExceptionを投げる。
  C_& at(int x, int y)
  {
    if (x < 0)
      throw polymnia::RangeOverException();
    if (x >= w_)
      throw polymnia::RangeOverException();
    if (y<0)
      throw polymnia::RangeOverException();
    if (y >= h_)
      throw polymnia::RangeOverException();

    return buf_[x + y * offset_];
  }

  /// @brief ピクセル(x, y)を參照
  ///
  /// pixel()は(x, y)がバッファの範圍に含まれるか確認しないが、
  /// at()は範圍を確認し、範圍外の場合はRangeOverExceptionを投げる。
  const C_& at(int x, int y) const
  {
    if (x < 0)
      throw polymnia::RangeOverException();
    if (x >= w_)
      throw polymnia::RangeOverException();
    if (y < 0)
      throw polymnia::RangeOverException();
    if (y >= h_)
      throw polymnia::RangeOverException();

    return buf_[x + y * offset_];
  }


  //======================================================================
  //  圖形描写
  //======================================================================
  /// @brief 直線を描畫
  void line(int x1, int y1, int x2, int y2, const C_& col);
  /// @brief 長方形を描畫
  void box(int x1, int y1, int x2, int y2, const C_& col, bool fl=false);
  /// @brief 楕圓を描畫
  void ellipse(int x, int y, int a, int b, const C_& col, bool fl=false);

  /// @brief 圓を描畫
  void circle(int x, int y, int r, const C_& col, bool fl=false)
  {
    ellipse(x, y, r, r, col, fl);
  }

  /// @brief 塗り潰し
  void paintFill(int x, int y, const C_& col);
  /// @brief バッファ全體の塗り潰し
  void clear(const C_& col);


  //===================================================================
  //  轉送
  //  tcol 附きのものは, tcol を透明色と看做して轉送
  //  mk 附きのものは, mk をマスクとして轉送
  //===================================================================
  /// @brief 轉送
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  void
  blt(
    int dx, int dy,
    const polymnia::ImageBuffer<C_>* src, int sx, int sy, int w, int h);

  /// @brief 轉送
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param mk 變更を許すマスク領域
  void blt(
    int dx, int dy,
    const polymnia::ImageBuffer<C_>* src, int sx, int sy, int w, int h,
    const polymnia::Rect& mk);

  /// @brief 轉送
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param tcol 透明色
  void blt(
    int dx, int dy,
    const polymnia::ImageBuffer<C_>* src, int sx, int sy, int w, int h,
    const C_& tcol);

  /// @brief 轉送
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param tcol 透明色
  /// @param mk 變更を許すマスク領域
  void blt(
    int dx, int dy,
    const polymnia::ImageBuffer<C_>* src, int sx, int sy, int w, int h,
    const C_& tcol, const polymnia::Rect& mk);

  /// @brief 色表現の異なるバッファ間の轉送
  ///
  /// 色表現型C_の異なる畫像バッファ間で轉送を行ふ。
  /// 轉送元の色表現を轉送先の色表現に暗默的に變換可能でなければならない。
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
    int w, int h);

  /// @brief 色表現の異なるバッファ間の轉送
  ///
  /// 色表現型C_の異なる畫像バッファ間で轉送を行ふ。
  /// 轉送元の色表現を轉送先の色表現に暗默的に變換可能でなければならない。
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param mk 變更を許すマスク領域
  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src,
    int sx, int sy, int w, int h, const polymnia::Rect& mk);

  /// @brief 色表現の異なるバッファ間の轉送
  ///
  /// 色表現型C_の異なる畫像バッファ間で轉送を行ふ。
  /// 轉送元の色表現を轉送先の色表現に暗默的に變換可能でなければならない。
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param tcol 透明色
  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src, int sx, int sy,
    int w, int h, const C2_& tcol);

  /// @brief 色表現の異なるバッファ間の轉送
  ///
  /// 色表現型C_の異なる畫像バッファ間で轉送を行ふ。
  /// 轉送元の色表現を轉送先の色表現に暗默的に變換可能でなければならない。
  /// @param dx 轉送先x座標
  /// @param dy 轉送先y座標
  /// @param src 轉送元バッファ
  /// @param sx 轉送元x座標
  /// @param sy 轉送元y座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param tcol 透明色
  /// @param mk 變更を許すマスク領域
  template<class C2_>
  void blt(
    int dx, int dy, const polymnia::ImageBuffer<C2_>* src,
    int sx, int sy, int w, int h, const C2_& tcol, const polymnia::Rect& mk);


  //===================================================
  //  エフェクトをかける
  //===================================================
  //void addEffect(const Effecter& efc);
};


#include "ibuf-imp_.h"


#endif // INCLUDE_GUARD_POLYMNIA_IMAGEBUFFER_H
