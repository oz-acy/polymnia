/**********************************************************************//**
 *
 *  @file hex.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief 正六角形マス描畫クラス
 *
 *  @date 2018.12.28 新規實裝
 */
#ifndef INC_POLYMNIA_HEX_H__
#define INC_POLYMNIA_HEX_H__

#include <algorithm>
#include <cmath>
#include "ibuf.h"

namespace polymnia
{
  template<class C_> class HexPainter;
}


/*------------------------------------------------*//**
 *  @brief 正六角形マス描畫クラス
 *
 *  内部狀態として以下の値を保持し、
 *  それらに基づいて正六角形の升目を描畫する。
 *  - R 正六角形の一邊のピクセル長
 *  - P0 マス(0, 0)の中心のピクセル水平座標
 *  - Q0 マス(0, 0)の中心のピクセル垂直座標
 */
template<class C_>
class polymnia::HexPainter
{
private:
  int r_; ///< HEXの一邊の長さR
  int p0_; ///< HEX(0, 0)の中心の水平座標P0
  int q0_; ///< HEX(0, 0)の中心の垂直座標Q0

  static inline const double SQRT3_ = std::sqrt(3);

public:
  /// @brief 構築子
  /// @param r HEXの邊の長さR
  /// @param p0 HEX(0, 0)の中心の水平座標P0
  /// @param q0 HEX(0, 0)の中心の垂直座標Q0
  HexPainter(int r, int p0, int q0) : r_(r), p0_(p0), q0_(q0) {}

  /// @brief 原點の再設定
  ///
  /// HEX(0, 0)の中心のピクセル座標(P0, Q0)を變更する。
  /// @param p0 新たなP0
  /// @param q0 新たなQ0
  void resetOrigin(int p0, int q0)
  {
    p0_ = p0;
    q0_ = q0;
  }

  /// @brief HEXの大きさの再設定
  ///
  /// HEXの一邊の長さ(=半徑)Rを變更する。
  /// @param r 新たなR
  void resetRadius(int r) { r_ = r; }

  /// @brief 原點の取得
  ///
  /// HEX(0, 0)の中心のピクセル座標(P0, Q0)を取得する。
  /// @param[out] p P0
  /// @param[out] q Q0
  void getOrigin(int& p, int& q) const
  {
    p = p0_;
    q = q0_;
  }

  /// @brief HEXの大きさの取得
  ///
  /// HEXの一邊の長さ(=半徑)Rを取得する。
  int getRadius() const { return r_; }

  /// @brief HEX外周の描畫
  ///
  /// HEX(x, y)の外周を色colで描畫する。
  /// @param pict 描畫對象のイメージバッファ
  /// @param x HEXの水平座標
  /// @param y HEXの垂直座標
  /// @param col HEX外周を描畫する色
  void draw(polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col);

  /// @brief 指定範圍のHEX外周の描畫
  ///
  /// HEX(x, y)-(x + w - 1, y + h - 1)の外周を色colで描畫する。
  /// @param pict 描畫對象のイメージバッファ
  /// @param x 領域左上のHEXの水平座標
  /// @param y 領域左上のHEXの垂直座標
  /// @param w 領域の幅
  /// @param h 領域の高さ
  /// @param col HEX外周を描畫する色
  void
  draw(
    polymnia::ImageBuffer<C_>* pict, int x, int y, int w, int h, const C_& col);

  /// @brief HEXの塗り潰し
  ///
  /// HEX(x, y)を色colで塗り潰す。
  /// @param pict 描畫對象のイメージバッファ
  /// @param x HEXのx座標
  /// @param y HEXのy座標
  /// @param col HEXを塗り潰す色
  void fill(polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col);

  /// @brief 指定範圍のHEXの塗り潰し
  ///
  /// HEX(x, y)を色colで塗り潰す。
  /// @param pict 描畫對象のイメージバッファ
  /// @param x 領域左上のHEXの水平座標
  /// @param y 領域左上のHEXの垂直座標
  /// @param w 領域の幅
  /// @param h 領域の高さ
  /// @param col HEXを塗り潰す色
  void
  fill(
    polymnia::ImageBuffer<C_>* pict, int x, int y, int w, int h, const C_& col);

  /// @brief HEX位置の取得
  ///
  /// ピクセル(p, q)を含むHEX(x, y)を取得する。
  /// @param[out] x HEXの水平座標
  /// @param[out] y HEXの垂直座標
  /// @param[in] p ピクセルの水平座標
  /// @param[in] q ピクセルの垂直座標
  void getHexPosition(int& x, int& y, int p, int q);

  /// @brief ピクセル座標の取得
  ///
  /// HEX(x, y)の中心のピクセル座標(p, q)を取得する。
  /// @param[out] p ピクセルの水平座標
  /// @param[out] q ピクセルの垂直座標
  /// @param[in] x HEXの水平座標
  /// @param[in] y HEXの垂直座標
  void getPixelPosition(int& p, int& q, int x, int y)
  {
    p = SQRT3_ * r_ * (x + 0.5 * (y % 2)) + p0_;
    q = (3.0 * y * r_) / 2.0 + q0_;
  }

  /// @brief ピクセル位置の取得
  ///
  /// HEX(x, y)の中心のピクセル座標(p, q)を取得する。
  /// @param[out] p ピクセルの水平座標
  /// @param[out] q ピクセルの垂直座標
  /// @param[in] x HEXの水平座標
  /// @param[in] y HEXの垂直座標
  void getPixelPosition(double& p, double& q, int x, int y)
  {
    p = SQRT3_ * r_ * (x + 0.5 * (y % 2)) + p0_;
    q = (3.0 * y * r_) / 2.0 + q0_;
  }

  /// @brief 距離の取得
  ///
  /// HEX(x0, y0)と(x1, y1)の距離を取得する。
  int distance(int x0, int y0, int x1, int y1)
  {
    int y0_2 = y0 / 2;
    if (y0 < 0)
      y0_2 -= 1;
  
    int y1_2 = y1 / 2;
    if (y1 < 0)
      y1_2 -= 1;
  
    int a = std::abs(x1 - x0 - y1_2 + y0_2);
    int b = std::abs(y1 - y0);
    int c = std::abs(x1 + y1 - y1_2 - x0 - y0 + y0_2);
  
    return std::max({a, b, c});
  }

private:
  /// @brief 頂點のピクセル座標の算出
  ///
  /// @param[out] p1 左邊水平座標
  /// @param[out] p2 右邊水平座標
  /// @param[out] q1 中央上頂點垂直座標
  /// @param[out] q2 左右邊上端垂直座標
  /// @param[out] q3 左右邊下端垂直座標
  /// @param[out] q4 中央下頂點垂直座標
  /// @param[in] p 中心水平座標 = 中央上下頂點垂直座標
  /// @param[in] q 中心垂直座標
  void
  calcVertex_(
    int& p1, int& p2, int& q1, int& q2, int& q3, int &q4,
    double p, double q)
  {
    // 頂點の座標: なほ、(p, q)は中心の座標
    //
    //          (p, q1)
    // (p1, q2)         (p2, q2)
    //          (p, q)
    // (p1, q3)         (p2, q3)
    //          (p, q4)
    //double p, q;
    p1 = p - SQRT3_ * r_ / 2.0;
    p2 = p + SQRT3_ * r_ / 2.0;

    q1 = q - r_;
    q2 = q - r_ / 2.0;
    q3 = q + r_ / 2.0;
    q4 = q + r_;
  }

  /// @brief HEXの左半の塗り潰し
  void fillLeftHalf_(
    polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col);
  /// @brief HEXの右半の塗り潰し
  void fillRightHalf_(
    polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col);
};




//============================
template<class C_>
inline
void
polymnia::HexPainter<C_>::draw(
  polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col)
{
  double p, q;
  getPixelPosition(p, q, x, y);

  int p1, p2;
  int q1, q2, q3, q4;
  calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

  // 座標を求めた後は直線を引く
  ////左側
  pict->line(p1, q2, p, q1, col);
  pict->line(p1, q2, p1, q3, col);
  pict->line(p1, q3, p, q4, col);
  ////右側
  pict->line(p2, q2, p, q1, col);
  pict->line(p2, q2, p2, q3, col);
  pict->line(p2, q3, p, q4, col);
}


//============================
template<class C_>
inline
void
polymnia::HexPainter<C_>::draw(
  polymnia::ImageBuffer<C_>* pict, int x, int y, int w, int h, const C_& col)
{
  double p, q;
  int p1, p2, q1, q2, q3, q4;

  for (int j = 0; j < h; j++) {

    // 左端のみ必要な描畫
    if ((y + j) % 2 == 0) {
      getPixelPosition(p, q, x, y + j);
      calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
      pict->line(p1, q3, p, q4, col);  // 左下斜め
    }

    // いつも必要な描畫
    for (int i = 0; i < w; i++) {
      getPixelPosition(p, q, x + i, y + j);
      calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

      pict->line(p1, q2, p, q1, col);  //左上斜め
      pict->line(p1, q2, p1, q3, col); //左端
      pict->line(p2, q2, p, q1, col);  //右上斜め
    }
    
    // 右端のみ必要な描畫
    getPixelPosition(p, q, x + w - 1, y + j);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pict->line(p2, q2, p2, q3, col);  // 右端
    if ((y + j) % 2 == 1) {
      pict->line(p2, q3, p, q4, col); // 右下斜め
    }
  }

  // 下端で必要な描畫
  for (int i = 0; i < w; i++) {
    getPixelPosition(p, q, x + i, y + h - 1);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pict->line(p1, q3, p, q4, col);  // 左下斜め
    pict->line(p2, q3, p, q4, col); // 右下斜め
  }
}


//============================
template<class C_>
inline
void
polymnia::HexPainter<C_>::fill(
  polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col)
{
  fillLeftHalf_(pict, x, y, col);
  fillRightHalf_(pict, x, y, col);
}

template<class C_>
inline
void
polymnia::HexPainter<C_>::fillLeftHalf_(
  polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col)
{
  // 頂點の座標の算出
  double p, q;
  getPixelPosition(p, q, x, y);
  int p1, p2;
  int q1, q2, q3, q4;
  calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

  // 塗り潰し
  int dp = (int)p - p1;
  int dq1 = q2 - q1;
  int dq2 = q4 - q3;

  int pp1 = p1;
  int pp2 = p;
  int qq1 = q2;
  int qq2 = q1;
  int qq3 = q3;
  int qq4 = q4;

  int e1 = -dp;
  int e2 = -dp;
  int lx = (dp + 1) / 2;
  for (int i = 0; i < lx; i++) {
    pict->line(pp1, qq1, pp1, qq3, col);
    pict->line(pp2, qq2, pp2, qq4, col);

    pp1++;
    pp2--;

    e1 += 2 * dq1;
    if (e1 >= 0) {
      qq1--;
      qq2++;
      e1 -= 2 * dp;
    }
    e2 += 2 * dq2;
    if (e2 >= 0) {
      qq3++;
      qq4--;
      e2 -= 2 * dp;
    }
  }
  if (!(dp % 2))
    pict->line(pp1, qq1, pp1, qq3, col);
}

template<class C_>
inline
void
polymnia::HexPainter<C_>::fillRightHalf_(
  polymnia::ImageBuffer<C_>* pict, int x, int y, const C_& col)
{
  // 頂點の座標の算出
  double p, q;
  getPixelPosition(p, q, x, y);
  int p1, p2;
  int q1, q2, q3, q4;
  calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

  // 塗り潰し
  int dp = p2 - (int)p;
  int dq1 = q2 - q1;
  int dq2 = q4 - q3;
  int pp1 = p;
  int pp2 = p2;
  int qq1 = q1;
  int qq2 = q2;
  int qq3 = q4;
  int qq4 = q3;

  int e1 = -dp;
  int e2 = -dp;
  int lx = (dp + 1) / 2;
  for (int i = 0; i < lx; i++) {
    pict->line(pp1, qq1, pp1, qq3, col);
    pict->line(pp2, qq2, pp2, qq4, col);

    pp1++;
    pp2--;

    e1 += 2 * dq1;
    if (e1 >= 0) {
      qq1++;
      qq2--;
      e1 -= 2 * dp;
    }
    e2 += 2 * dq2;
    if (e2 >= 0) {
      qq3--;
      qq4++;
      e2 -= 2 * dp;
    }
  }
  if (!(dp % 2))
    pict->line(pp1, qq1, pp1, qq3, col);
}


//============================
template<class C_>
inline
void
polymnia::HexPainter<C_>::fill(
  polymnia::ImageBuffer<C_>* pict, int x, int y, int w, int h, const C_& col)
{
  // 使ひ廻す變數
  double p, q;
  int p1, p2;
  int q1, q2, q3, q4;


  // 中央の「箱」の塗り潰し
  int pbox0, qbox0, pbox1, qbox1;
  if (y % 2 == 0) {
    getPixelPosition(p, q, x, y);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pbox0 = p;
    qbox0 = q2;

    getPixelPosition(p, q, x + w - 1, y);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pbox1 = p2;

    getPixelPosition(p, q, x + w - 1, y + h - 1);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    qbox1 = q3;
  } else {
    getPixelPosition(p, q, x, y);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pbox0 = p1;
    qbox0 = q2;

    getPixelPosition(p, q, x + w - 1, y);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    pbox1 = p;

    getPixelPosition(p, q, x + w - 1, y + h - 1);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);
    qbox1 = q3;
  }
  pict->box(pbox0, qbox0, pbox1, qbox1, col, true);

  // 左端の塗り潰し
  for (int j = 0; j < h; j++) {
    if ((y + j) % 2 == 0) {
      fillLeftHalf_(pict, x, y + j, col);
    }
  }

  // 右端の塗り潰し
  for (int j = 0; j < h; j++) {
    if ((y + j) % 2 != 0) {
      fillRightHalf_(pict, x + w - 1, y + j, col);
    }
  }

  for (int i = 0; i < w; i++) {
    int dp, dq; //dq1, dq2;
    int pp1, pp2, qq1, qq2; //, qq3, qq4;
    int e, lx;

    // 上端三角形の塗り潰し
    getPixelPosition(p, q, x + i, y);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

    //// 左半分の塗り潰し
    dp = (int)p - p1;
    dq = q2 - q1;

    pp1 = p1;
    pp2 = p;
    qq1 = q2;
    qq2 = q1;

    e = -dp;
    lx = (dp + 1) / 2;
    for (int i = 0; i < lx; i++) {
      pict->line(pp1, qq1, pp1, q2, col);
      pict->line(pp2, qq2, pp2, q2, col);

      pp1++;
      pp2--;
      e += 2 * dq;
      if (e >= 0) {
        qq1--;
        qq2++;
        e -= 2 * dp;
      }
    }
    if (!(dp % 2))
      pict->line(pp1, qq1, pp1, q2, col);

    //// 右半分の塗り潰し
    dp = p2 - (int)p;
    ////dqは左半分と全く同じ
    pp1 = p;
    pp2 = p2;
    qq1 = q1;
    qq2 = q2;

    e = -dp;
    lx = (dp + 1) / 2;
    for (int i = 0; i < lx; i++) {
      pict->line(pp1, qq1, pp1, q2, col);
      pict->line(pp2, qq2, pp2, q2, col);

      pp1++;
      pp2--;

      e += 2 * dq;
      if (e >= 0) {
        qq1++;
        qq2--;
        e -= 2 * dp;
      }
    }
    if (!(dp % 2))
      pict->line(pp1, qq1, pp1, q2, col);


    // 下端三角形の塗り潰し
    getPixelPosition(p, q, x + i, y + h - 1);
    calcVertex_(p1, p2, q1, q2, q3, q4, p, q);

    //// 左半分の塗り潰し
    dp = (int)p - p1;
    dq = q4 - q3;

    pp1 = p1;
    pp2 = p;
    qq1 = q3;
    qq2 = q4;

    e = -dp;
    lx = (dp + 1) / 2;
    for (int i = 0; i < lx; i++) {
      pict->line(pp1, q3, pp1, qq1, col);
      pict->line(pp2, q3, pp2, qq2, col);

      pp1++;
      pp2--;

      e += 2 * dq;
      if (e >= 0) {
        qq1++;
        qq2--;
        e -= 2 * dp;
      }
    }
    if (!(dp % 2))
      pict->line(pp1, q3, pp1, qq1, col);

    //// 右半分
    dp = p2 - (int)p;
    ////dqは左半分と全く同じ
    pp1 = p;
    pp2 = p2;
    qq1 = q4;
    qq2 = q3;

    e = -dp;
    lx = (dp + 1) / 2;
    for (int i = 0; i < lx; i++) {
      pict->line(pp1, q3, pp1, qq1, col);
      pict->line(pp2, q3, pp2, qq2, col);

      pp1++;
      pp2--;

      e += 2 * dq;
      if (e >= 0) {
        qq1--;
        qq2++;
        e -= 2 * dp;
      }
    }
    if (!(dp % 2))
      pict->line(pp1, q3, pp1, qq1, col);

  }//end of for
}




//============================
template<class C_>
inline
void
polymnia::HexPainter<C_>::getHexPosition(int& x, int& y, int p, int q)
{
  // []はHEX ()はピクセル
  // 長方形を五つのHEX(の全部または一部)に分割する。
  // 長方形左上を(0, 0)とし、差分(dp, dq)を求めて判定する。
  //
  // [x0, y0] (√3*R, 0)     [x0+1, y0]
  //          (√3*R, R/2)
  // (0, R)                 (2*√3*R, R)
  //           [x0, y0+1]
  // (0, 2*R)               (2*√3*R, 2*R)
  //          (√3*R, 5*R/2)
  // [x0, y0+2] (√3*R, 3*R)  [x0+1, y0+2]

  int qq = q - q0_;
  int y0 = std::floor(qq / 3.0 / r_) * 2;
  double dq = qq - (y0 / 2) * 3 * r_;

  int pp = p - p0_;
  int x0 = std::floor(pp * SQRT3_ / 3.0 / r_);
  double dp = pp - x0 * SQRT3_ * r_;

  double u = -1 * dp * SQRT3_ / 3;
  double v = dp * SQRT3_ / 3;

  if ((dq < u + r_) || (dq < v)) {
    if (dp < SQRT3_ * r_ / 2) {
      x = x0;
    } else {
      x = x0 + 1;
    }
    y = y0;
  }
  else if ((dq >= u + 3 * r_) || (dq >= v + 2 * r_)) {
    if (dp < SQRT3_ * r_ / 2) {
      x = x0;
    } else {
      x = x0 + 1;
    }
    y = y0 + 2;
  }
  else {
    x = x0;
    y = y0 + 1;
  }
}




#endif // INC_POLYMNIA_HEX_H__
