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
 * @file toindexed.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief PictureからPictureIndexedへの減色複寫處理
 *
 * @date 2016.2.26 ファイル名變更、他修正
 * @date 2016.3.2  throw()削除
 * @date 2018.12.28
 *     reducePictureColors()をPicture::duplicatePictureIndexed()に變更
 * @date 2019.8.27  __を含む名前を修正
 * @date 2019.8.29  duplicatePictureIndexedの返却型をunique_ptrに變更
 * @date 2021.3.25  themis/inttype.hの非推奬化に對應
 *
 */
#include <memory>
#include "picture.h"


namespace
{

/*///////////////////////////////////////////////
 *
 *  メディアンカットによるパレット作成ルーチン
 *
 *////////////////////////////////////////////*/

/*-------------------------------------------
 *  struct ColorRange_
 *  色要素についてのレンジ情報
 *-----------------------------------------*/
struct ColorRange_
{
public:
  std::uint8_t min;
  std::uint8_t max;
  std::uint32_t bits[8]; // exist x in min..max <=> bits[x>>5]<x&0x1f> = 1
  std::uint32_t sum;     // pixel値の總計

public:
  int cutoff();
};


/*==========================================================================
 *  ColorRange_::cutoff()
 *  ヒストグラム切り詰め (バウンディング・ボックスをとる)
 *  返値 : ヒストグラム幅
 *========================================================================*/
int ColorRange_::cutoff()
{
  using namespace themis;

  int i = 0, j, k;

  // 最小値を切り上げていく
  for (j = min >> 5; j < 8; j++)
    if (bits[j]) {
      for (i = 0, k = 1; i < 32 && !(bits[j] & k); i++, k <<= 1)
        ;
      break;
    }

  i += j << 5;
  if (i <= (int)max)
    min = (std::uint8_t)i;

  // 最大値を切り下げていく
  for (j = max >> 5; j >= 0; j--)
    if (bits[j]) {
      for (i = 31, k = 1 << 31; i >= 0 && !(bits[j] & k); i--, k >>= 1)
        ;
      break;
    }

  i += j << 5;
  if (i >= (int)min)
    max = (std::uint8_t)i;

  // 區間の幅を返す
  return max - min + 1;
}


/*----------------------------------------------------------
*  struct PixelRange_
*  メディアンカットで用ゐるヒストグラムのレンジ情報
*---------------------------------------------------------*/
struct PixelRange_
{
  unsigned count;        // レンジ内の畫素數
  ColorRange_ r, g, b;   // 各色要素のレンジ情報

public:
  void divide(PixelRange_& neu);
};

/*=============================================================
 *  PixelRange_::divide()
 *
 *  クラスタ分割
 *
 *  *this を *this と neu に二分割する
 *===========================================================*/
void PixelRange_::divide(PixelRange_& neu)
{
  using namespace themis;

  int rw = r.cutoff();
  int gw = g.cutoff();
  int bw = b.cutoff();
  if (count == 0)
    count = 1;

  neu = *this;

  if (gw >= bw && gw >= rw) {
    g.max = (std::uint8_t)(g.sum / count);
    neu.g.min = (std::uint8_t)(g.sum / count + 1);
  }
  else if(rw>=bw && rw>=gw) {
    r.max = (std::uint8_t)(r.sum / count);
    neu.r.min = (std::uint8_t)(r.sum / count + 1);
  }
  else {
    b.max = (std::uint8_t)(b.sum / count);
    neu.b.min = (std::uint8_t)(b.sum / count + 1);
  }
}



/*========================================================================
 *  sumpling_()
 *  サンプリング
 *======================================================================*/
void sumpling_(polymnia::RgbColor pal[], const polymnia::Picture* src)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  PixelRange_ range[256];

  memset(range, 0, sizeof(PixelRange_) * 256);
  range[0].r.max = range[0].g.max = range[0].b.max = 255;

  int i;
  int current_range = 1;

  for (int bits = 0; bits <= 8; bits++) {
    for (i = 0; i < current_range; i++) {
      memset(range[i].r.bits, 0, 32);
      memset(range[i].g.bits, 0, 32);
      memset(range[i].b.bits, 0, 32);
      range[i].r.sum = 0;
      range[i].g.sum = 0;
      range[i].b.sum = 0;
      range[i].count = 0;
    }

    for (int y = 0; y < src->height(); y++) {
      for (int x = 0; x < src->width(); x++) {
        std::uint8_t r = src->pixel(x, y).r;
        std::uint8_t g = src->pixel(x, y).g;
        std::uint8_t b = src->pixel(x, y).b;

        // r,g,bが属するrange#を得る
        for (i = 0; i < current_range; i++) {
          if (r < range[i].r.min || r > range[i].r.max)
            continue;
          if (g < range[i].g.min || g > range[i].g.max)
            continue;
          if (b < range[i].b.min || b > range[i].b.max)
            continue;

          break;
        }

        // Pixel値の合計
        range[i].r.sum += r;
        range[i].g.sum += g;
        range[i].b.sum += b;

        // Pixel値(r,g,b)の存在フラグを立てる
        range[i].r.bits[r >> 5] |= (std::uint32_t)1 << (r & 0x1F);
        range[i].g.bits[g >> 5] |= (std::uint32_t)1 << (g & 0x1F);
        range[i].b.bits[b >> 5] |= (std::uint32_t)1 << (b & 0x1F);

        // Pixel数
        range[i].count += 1;
      }
    }

    if (bits == 8)
      break;

    int stop = current_range;
    for (i = 0; i < stop; i++)
      range[i].divide(range[current_range++]);
  }

  for (i = 0; i < 256; i++) {
    if (range[i].count==0)
      range[i].count = 1;
    pal[i].r = (std::uint8_t)(range[i].r.sum / range[i].count);
    pal[i].g = (std::uint8_t)(range[i].g.sum / range[i].count);
    pal[i].b = (std::uint8_t)(range[i].b.sum / range[i].count);
  }
}// end of sampling_()


constexpr int NPAL = 256;
constexpr int SAMPLE_MAX = NPAL - 1;
constexpr int NCLASS = SAMPLE_MAX * 3 + 1;


/*==========================================
 *  rms_()
 *  RGB各成分の差の二乗の和を求める
 *========================================*/
constexpr long pow2_G[NPAL]
  =
{
  0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225,
  256, 289, 324, 361, 400, 441, 484, 529, 576, 625, 676, 729,
  784, 841, 900, 961, 1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521,
  1600, 1681, 1764, 1849, 1936, 2025, 2116, 2209, 2304, 2401, 2500, 2601,
  2704, 2809, 2916, 3025, 3136, 3249, 3364, 3481, 3600, 3721, 3844, 3969,
  4096, 4225, 4356, 4489, 4624, 4761, 4900, 5041, 5184, 5329, 5476, 5625,
  5776, 5929, 6084, 6241, 6400, 6561, 6724, 6889, 7056, 7225, 7396, 7569,
  7744, 7921, 8100, 8281, 8464, 8649, 8836, 9025, 9216, 9409, 9604, 9801,
  10000, 10201, 10404, 10609, 10816, 11025, 11236, 11449, 11664, 11881,
  12100, 12321, 12544, 12769, 12996, 13225, 13456, 13689, 13924, 14161,
  14400, 14641, 14884, 15129, 15376, 15625, 15876, 16129, 16384, 16641,
  16900, 17161, 17424, 17689, 17956, 18225, 18496, 18769, 19044, 19321,
  19600, 19881, 20164, 20449, 20736, 21025, 21316, 21609, 21904, 22201,
  22500, 22801, 23104, 23409, 23716, 24025, 24336, 24649, 24964, 25281,
  25600, 25921, 26244, 26569, 26896, 27225, 27556, 27889, 28224, 28561,
  28900, 29241, 29584, 29929, 30276, 30625, 30976, 31329, 31684, 32041,
  32400, 32761, 33124, 33489, 33856, 34225, 34596, 34969, 35344, 35721,
  36100, 36481, 36864, 37249, 37636, 38025, 38416, 38809, 39204, 39601,
  40000, 40401, 40804, 41209, 41616, 42025, 42436, 42849, 43264, 43681,
  44100, 44521, 44944, 45369, 45796, 46225, 46656, 47089, 47524, 47961,
  48400, 48841, 49284, 49729, 50176, 50625, 51076, 51529, 51984, 52441,
  52900, 53361, 53824, 54289, 54756, 55225, 55696, 56169, 56644, 57121,
  57600, 58081, 58564, 59049, 59536, 60025, 60516, 61009, 61504, 62001,
  62500, 63001, 63504, 64009, 64516, 65025,
};

inline
unsigned long
rms_(const polymnia::RgbColor& c1, const polymnia::RgbColor& c2)
{
  return 
      pow2_G[std::abs(c1.r - c2.r)]
    + pow2_G[std::abs(c1.g - c2.g)]
    + pow2_G[std::abs(c1.b - c2.b)];
}


/* sumdif_G[i] は、 RGB総和の差が i である 2つの RGB値の 最小距離 */
const unsigned long sumdif_G[NCLASS]
  =
{
  0, 0, 1, 3, 5, 8, 12, 16, 21, 27, 33, 40, 48, 56, 65, 75, 85, 96, 108, 120, 
  133, 147, 161, 176, 192, 208, 225, 243, 261, 280, 300, 320, 341, 363, 385, 
  408, 432, 456, 481, 507, 533, 560, 588, 616, 645, 675, 705, 736, 768, 800,
  833, 867, 901, 936, 972, 1008, 1045, 1083, 1121, 1160, 1200, 1240, 1281,
  1323, 1365, 1408, 1452, 1496, 1541, 1587, 1633, 1680, 1728, 1776, 1825,
  1875, 1925, 1976, 2028, 2080, 2133, 2187, 2241, 2296, 2352, 2408, 2465,
  2523, 2581, 2640, 2700, 2760, 2821, 2883, 2945, 3008, 3072, 3136, 3201,
  3267, 3333, 3400, 3468, 3536, 3605, 3675, 3745, 3816, 3888, 3960, 4033,
  4107, 4181, 4256, 4332, 4408, 4485, 4563, 4641, 4720, 4800, 4880, 4961,
  5043, 5125, 5208, 5292, 5376, 5461, 5547, 5633, 5720, 5808, 5896, 5985,
  6075, 6165, 6256, 6348, 6440, 6533, 6627, 6721, 6816, 6912, 7008, 7105,
  7203, 7301, 7400, 7500, 7600, 7701, 7803, 7905, 8008, 8112, 8216, 8321,
  8427, 8533, 8640, 8748, 8856, 8965, 9075, 9185, 9296, 9408, 9520, 9633,
  9747, 9861, 9976, 10092, 10208, 10325, 10443, 10561, 10680, 10800, 10920,
  11041, 11163, 11285, 11408, 11532, 11656, 11781, 11907, 12033, 12160, 12288,
  12416, 12545, 12675, 12805, 12936, 13068, 13200, 13333, 13467, 13601, 13736,
  13872, 14008, 14145, 14283, 14421, 14560, 14700, 14840, 14981, 15123, 15265,
  15408, 15552, 15696, 15841, 15987, 16133, 16280, 16428, 16576, 16725, 16875,
  17025, 17176, 17328, 17480, 17633, 17787, 17941, 18096, 18252, 18408, 18565,
  18723, 18881, 19040, 19200, 19360, 19521, 19683, 19845, 20008, 20172, 20336,
  20501, 20667, 20833, 21000, 21168, 21336, 21505, 21675, 21845, 22016, 22188,
  22360, 22533, 22707, 22881, 23056, 23232, 23408, 23585, 23763, 23941, 24120,
  24300, 24480, 24661, 24843, 25025, 25208, 25392, 25576, 25761, 25947, 26133,
  26320, 26508, 26696, 26885, 27075, 27265, 27456, 27648, 27840, 28033, 28227,
  28421, 28616, 28812, 29008, 29205, 29403, 29601, 29800, 30000, 30200, 30401,
  30603, 30805, 31008, 31212, 31416, 31621, 31827, 32033, 32240, 32448, 32656,
  32865, 33075, 33285, 33496, 33708, 33920, 34133, 34347, 34561, 34776, 34992,
  35208, 35425, 35643, 35861, 36080, 36300, 36520, 36741, 36963, 37185, 37408,
  37632, 37856, 38081, 38307, 38533, 38760, 38988, 39216, 39445, 39675, 39905,
  40136, 40368, 40600, 40833, 41067, 41301, 41536, 41772, 42008, 42245, 42483,
  42721, 42960, 43200, 43440, 43681, 43923, 44165, 44408, 44652, 44896, 45141,
  45387, 45633, 45880, 46128, 46376, 46625, 46875, 47125, 47376, 47628, 47880,
  48133, 48387, 48641, 48896, 49152, 49408, 49665, 49923, 50181, 50440, 50700,
  50960, 51221, 51483, 51745, 52008, 52272, 52536, 52801, 53067, 53333, 53600,
  53868, 54136, 54405, 54675, 54945, 55216, 55488, 55760, 56033, 56307, 56581,
  56856, 57132, 57408, 57685, 57963, 58241, 58520, 58800, 59080, 59361, 59643,
  59925, 60208, 60492, 60776, 61061, 61347, 61633, 61920, 62208, 62496, 62785,
  63075, 63365, 63656, 63948, 64240, 64533, 64827, 65121, 65416, 65712, 66008,
  66305, 66603, 66901, 67200, 67500, 67800, 68101, 68403, 68705, 69008, 69312,
  69616, 69921, 70227, 70533, 70840, 71148, 71456, 71765, 72075, 72385, 72696,
  73008, 73320, 73633, 73947, 74261, 74576, 74892, 75208, 75525, 75843, 76161,
  76480, 76800, 77120, 77441, 77763, 78085, 78408, 78732, 79056, 79381, 79707,
  80033, 80360, 80688, 81016, 81345, 81675, 82005, 82336, 82668, 83000, 83333,
  83667, 84001, 84336, 84672, 85008, 85345, 85683, 86021, 86360, 86700, 87040,
  87381, 87723, 88065, 88408, 88752, 89096, 89441, 89787, 90133, 90480, 90828,
  91176, 91525, 91875, 92225, 92576, 92928, 93280, 93633, 93987, 94341, 94696,
  95052, 95408, 95765, 96123, 96481, 96840, 97200, 97560, 97921, 98283, 98645,
  99008, 99372, 99736, 100101, 100467, 100833, 101200, 101568, 101936, 102305,
  102675, 103045, 103416, 103788, 104160, 104533, 104907, 105281, 105656,
  106032, 106408, 106785, 107163, 107541, 107920, 108300, 108680, 109061,
  109443, 109825, 110208, 110592, 110976, 111361, 111747, 112133, 112520,
  112908, 113296, 113685, 114075, 114465, 114856, 115248, 115640, 116033,
  116427, 116821, 117216, 117612, 118008, 118405, 118803, 119201, 119600,
  120000, 120400, 120801, 121203, 121605, 122008, 122412, 122816, 123221,
  123627, 124033, 124440, 124848, 125256, 125665, 126075, 126485, 126896,
  127308, 127720, 128133, 128547, 128961, 129376, 129792, 130208, 130625,
  131043, 131461, 131880, 132300, 132720, 133141, 133563, 133985, 134408,
  134832, 135256, 135681, 136107, 136533, 136960, 137388, 137816, 138245,
  138675, 139105, 139536, 139968, 140400, 140833, 141267, 141701, 142136,
  142572, 143008, 143445, 143883, 144321, 144760, 145200, 145640, 146081,
  146523, 146965, 147408, 147852, 148296, 148741, 149187, 149633, 150080,
  150528, 150976, 151425, 151875, 152325, 152776, 153228, 153680, 154133,
  154587, 155041, 155496, 155952, 156408, 156865, 157323, 157781, 158240,
  158700, 159160, 159621, 160083, 160545, 161008, 161472, 161936, 162401,
  162867, 163333, 163800, 164268, 164736, 165205, 165675, 166145, 166616,
  167088, 167560, 168033, 168507, 168981, 169456, 169932, 170408, 170885,
  171363, 171841, 172320, 172800, 173280, 173761, 174243, 174725, 175208,
  175692, 176176, 176661, 177147, 177633, 178120, 178608, 179096, 179585,
  180075, 180565, 181056, 181548, 182040, 182533, 183027, 183521, 184016,
  184512, 185008, 185505, 186003, 186501, 187000, 187500, 188000, 188501,
  189003, 189505, 190008, 190512, 191016, 191521, 192027, 192533, 193040,
  193548, 194056, 194565, 195075
};


/*
 *  RGB成分の和が k であるパレットについて片方向リンクを張る。
 *  index_G[k] が リンクの先頭のパレットの番号になる。
 *  next_G[pal] が pal の次のパレット番号になる。
 *  next_G[]を次々辿り、 next_G[k] が -1 になったところがリンクの終點。
 *
 *  なほ、index_G[k] = -1 なら、成分和が k であるパレットはない。
 */
int next_G[NPAL];
int index_G[NCLASS];

/* next_G[], index_G[] によるパレット探索鎖を生成する */
void updateChain_(const polymnia::RgbColor pal[])
{
  for (int i = 0; i < NCLASS; i++)
    index_G[i] = -1;

  for (int i = 0; i < NPAL; i++) {
    int k = pal[i].r + pal[i].g + pal[i].b;
    next_G[i] = index_G[k];
    index_G[k] = i;
  }
}


/* 與RGB値の最近パレットを求める */
std::uint8_t findNearestPal_(
  const polymnia::RgbColor& col, const polymnia::RgbColor pal[])
{
  using namespace themis;

  // 現在のところの距離の最小値 : "最も大きい整数" で初期化
  unsigned long memdif = ~0;

  // 現在のところの最近パレット : 取りあへず 0で初期化
  std::uint8_t mempos = 0;

  // 與RGB値の成分和
  int base = col.r + col.g + col.b;


  /*
   *  探索對象を
   *  與 RGB 値との RGB 成分差の和が
   *  小さいところから大きいところに擴げながら調べる
   */
  for (int absval = 0; absval < NCLASS; absval++) {
    /*
     *  現在の最近パレットとの距離より
     *  これから調べる對象 (の最小値) との距離の方が
     *  大きいときは打ち切る。
     *
     *  (成分差の和と 距離との関係より 求まる : subdif_G[]に格納済)
     */
    if (memdif < sumdif_G[absval])
      break;

    unsigned long dif;  // col と 比較対象の 差の二乗の和


    /* RGBの總和が col より absval だけ大きいパレットを調べて廻る */
    int cc = (base+absval < NCLASS) ? index_G[base + absval] : -1;
    while (cc >= 0 && cc < NPAL) {
      dif = rms_(col, pal[cc]);
      if (dif == 0)
        return (std::uint8_t)cc;  // 距離 0 なら 文句なし終了
      else if (dif<memdif)
      {
        // 最近パレットと最小距離を付け替え
        mempos = (std::uint8_t)cc;
        memdif = dif;
      }
      cc = next_G[cc];  // リンクを辿る
    }


    /* RGBの總和が col より absval だけ小さいパレットを調べて廻る */
    cc = (base - absval >= 0) ? index_G[base - absval] : -1;
    while (cc >= 0 && cc < NPAL) {
      dif = rms_(col, pal[cc]);
      if (dif == 0)
        return (std::uint8_t)cc;  // 距離 0 なら 文句なし終了
      else if (dif < memdif) {
        // 最近パレットと最小距離を付け替え
        mempos = (std::uint8_t)cc;
        memdif = dif;
      }
      cc = next_G[cc];  // リンクを辿る
    }
  }

  return mempos;  /* 最終的な最近パレット番號を返す */
}// end of findNearestPal_()



#define DIFFUSE 1


#if DIFFUSE
  /* 誤差擴散法による減色處理 */
#define PATX 5     // パターン配列の幅
#define PATY 3     // パターン配列の高さ
#define D_AREA 2   // 分散するPixelの範圍
#define ERR_PTN { 0, 0, 0, 7, 5,  3, 5, 7, 5, 3,  1, 3, 5, 3, 1 }
void deColor_(polymnia::PictureIndexed* dst, const polymnia::Picture* src)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  //int i;

  // バッファの確保と初期化
  int mx = src->width() + D_AREA * 2;
  int sum = mx * PATY;
  
  auto rerr = std::make_unique<int[]>(sum);
  auto gerr = std::make_unique<int[]>(sum);
  auto berr = std::make_unique<int[]>(sum);
  memset(rerr.get(), 0, sizeof(int) * sum);
  memset(gerr.get(), 0, sizeof(int) * sum);
  memset(berr.get(), 0, sizeof(int) * sum);

  // 分散パターン
  int err_pat[PATX*PATY] = ERR_PTN;
  int pat_sum = 0;
  for (int i = 0; i < PATX * PATY; i++)
    pat_sum += err_pat[i];


  for (int y = 0; y < src->height(); y++) {
    for (int x = 0; x < src->width(); x++) {
      RgbColor col = src->pixel(x, y);
      int adr = x + D_AREA;
      int rr = col.r + rerr[adr]/pat_sum;
      int gg = col.g + gerr[adr]/pat_sum;
      int bb = col.b + berr[adr]/pat_sum;

      int r2 = rr > 255 ? 255 : rr;   r2 = r2 < 0 ? 0 : r2;
      int g2 = gg > 255 ? 255 : gg;   g2 = g2 < 0 ? 0 : g2;
      int b2 = bb > 255 ? 255 : bb;   b2 = b2 < 0 ? 0 : b2;

      std::uint8_t bst
        = findNearestPal_(
            RgbColor((std::uint8_t)r2, (std::uint8_t)g2, (std::uint8_t)b2),
            dst->paletteBuffer());

      int re = rr - dst->palette(bst).r;
      int ge = gg - dst->palette(bst).g;
      int be = bb - dst->palette(bst).b;


      // 誤差分散
      adr -= D_AREA;
      for (int iy = 0; iy < PATY; iy++, adr+=mx) {
        for (int ix = 0; ix < PATX; ix++) {
          rerr[adr + ix] += re * err_pat[ix + iy * PATX];
          gerr[adr + ix] += ge * err_pat[ix + iy * PATX];
          berr[adr + ix] += be * err_pat[ix + iy * PATX];
        }
      }

      dst->pixel(x, y) = bst;
    }

    // バッファのずらし處理
    for (int i = 0; i < mx; i++) {
      for (int j = 0; j < PATY-1; j++) {
        rerr[i + j * mx] = rerr[i + (j+1) * mx];
        gerr[i + j * mx] = gerr[i + (j+1) * mx];
        berr[i + j * mx] = berr[i + (j+1) * mx];
      }
      rerr[i + (PATY-1) * mx] = 0;
      gerr[i + (PATY-1) * mx] = 0;
      berr[i + (PATY-1) * mx] = 0;
    }
  }
}
#else
  // 單純近似による減色
void
deColor_(const polymnia::PictureIndexed* dst, const polymnia::Picture* src)
{
  using namespace themis;

  for (int y = 0; y < src->height(); y++) {
    for (int x = 0; x < src->width(); x++) {
      std::uint8_t bst
        = findNearestPal_(src->pixel(x, y), dst->paletteBuffer());
      dst->pixel(x, y) = bst;
    }
  }
}
#endif



}// end of namespace


/*==========================================================
 *  Picture::duplicatePictureIndexed()
 *
 *  Pictureに減色處理を施して
 *  PictureIndexedオブジェクトを生成する。
 */
std::unique_ptr<polymnia::PictureIndexed>
polymnia::Picture::duplicatePictureIndexed() const noexcept
{
  auto pc = PictureIndexed::create(w_, h_);
  if (!pc)
    return nullptr;

  sumpling_(pc->paletteBuffer(), this);
  updateChain_(pc->paletteBuffer());
  deColor_(pc.get(), this);

  return pc;
}


//eof
