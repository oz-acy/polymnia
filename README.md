# project Polymnia

## これはなに?
畫像データを扱ふためのライブラリ。

## Polymniaとは?
[女神ポリュムニアー](https://ja.wikipedia.org/wiki/%E3%83%9D%E3%83%AA%E3%83%A5%E3%83%A0%E3%83%8B%E3%82%A2%E3%83%BC)の名を拜借した。

## 内容

### 畫像バッファ
ピクセル表現をテンプレートパラメタとするクラステンプレートと、
RGB24bitColor、256 Indexed Colorの特殊化を提供する。

### 畫像バッファの入出力
DIB、PNG、JPEGの入出力に對應。とはいへ、各フォーマットの機能の全てを網羅してゐるわけではない。

### 畫像バッファに對する處理
次の處理を實裝してゐる。
* グレイスケール化
* 24bitColorから256 Indexed Colorへの減色
* 擴大、縮小


## 依存してゐるライブラリ
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
    target_link_libraries(targetname themis polymnia ...)

のやうに書けば、include pathやlibrary pathを設定し、ライブラリをリンクしてくれる、はず。

ヘッダファイルのincludeは

    #include <themis/except.h>

のやうにする。


## ライセンス
[二條項BSDライセンス](https://opensource.org/licenses/BSD-2-Clause)とする。


## 作者
oZ/acy (名賀月晃嗣)
* <acy@hiemalis.org>
* <http://www.hiemalis.org>
