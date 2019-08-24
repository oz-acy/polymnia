# project Polymnia

## これはなに?
畫像データを扱ふためのライブラリ。

## Polymniaとは?
[女神ポリュムニアー](https://ja.wikipedia.org/wiki/%E3%83%9D%E3%83%AA%E3%83%A5%E3%83%A0%E3%83%8B%E3%82%A2%E3%83%BC)の名を拜借した。

## 内容
ライブラリの提供する機能を、ヘッダファイル毎に示す。
|ヘッダファイル|内容|
|:---|:---|
|polymnia/ibuf.h|ピクセル表現をテンプレートパラメタとする畫像バッファクラステンプレート|
|polymnia/rgb.h|RGB24bitColor表現クラス|
|polymnia/picture.h|RGB24bitColorと256IndexedColorの畫像バッファクラス|
|polymnia/pictio.h|畫像入出力基底クラス|
|polymnia/dibio.h|DIB形式の入出力|
|polymnia/pngio.h|PNG形式の入出力|
|polymnia/jpegio.h|JPEG形式の入出力|
|polymnia/hex.h|ヘックスマップ描畫用クラステンプレート|

ライブラリの外、ライブラリを用ゐた畫像擴大縮小用のコンソールアプリケーションresizerを提供する。

### 畫像バッファ
ピクセル表現をテンプレートパラメタとするクラステンプレートと、
それを基底とする
RGB24bitColor、256IndexedColorの畫像バッファを提供する。

24bitColor畫像バッファは、
256IndexedColorへの減色、グレイスケール化(結果は256IndexedColor)、擴大、縮小の
各處理を實裝する。

### 畫像バッファの入出力
DIB、PNG、JPEGの入出力に對應。とはいへ、各フォーマットの機能の全てを網羅してゐるわけではない。

### ヘックスマップ描畫
畫像バッファクラステンプレートに對して
シミュレーションゲームで見られるヘックスマップを描畫するためのクラステンプレートを提供する。

### resizer
libpolymniaを用ゐた畫像の擴大縮小を行ふコンソールアプリケーション。
次のやうに對象ファイルやオプションを指定して使用する。

    $ resizer -p &lt;倍率&gt; &lt;入力ファイル&gt; &lt;出力ファイル&gt;
    $ resizer -l &lt;處理後の畫像の幅&gt; &lt;處理後の畫像の高さ&gt; &lt;入力ファイル&gt; &lt;出力ファイル&gt;

畫像フォーマットは擴張子で判別する。出力ファイルのフォーマットが入力ファイルとは違つてゐても構はない。
對應する擴張子は、.bmp、.png、.jpg、.jpeg。
出力ファイルの擴張子がこれらと異なる場合でも、一往はDIB(BMP)形式で保存される。


## 依存してゐるライブラリ
* [Boost](https://www.boost.org/)
* [libpng](http://www.libpng.org/pub/png/libpng.html)
* [zlib](https://www.zlib.net/)
* [libjpeg](http://libjpeg.sourceforge.net/)
* [libthemis](https://github.com/oz-acy/themis)

(自身を除く)各ライブラリの作者には謝意と敬意を表する。


# 使ひ方
まづ、依存ライブラリをインストールする。

それから、CMakeを用ゐてインストールする。

    $ cmake -G "Unix Makefiles" .
    $ make
    $ make install

ジェネレータは環境に應じて適宜變更すること。他にもcmakeにあれこれオプション指定する必要があるかもしれない。

利用するときには、CMakeLists.txtに

    find_package(themis REQUIRED)
    find_package(polymnia REQUIRED)
    # ...
    target_link_libraries(targetname polymnia themis ...)

のやうに書けば、include pathやlibrary pathを設定し、ライブラリをリンクしてくれる、はず。

ヘッダファイルのincludeは

    #include <polymnia/picture.h>

のやうにする。


## ライセンス
[二條項BSDライセンス](https://opensource.org/licenses/BSD-2-Clause)とする。


## 作者
oZ/acy (名賀月晃嗣)
* <acy@hiemalis.org>
* <http://www.hiemalis.org>
