# Hopfield network

## Hopfield network とは
Hopfield network（ホップフィールド・ネットワーク）またの名を associative memory（連想記憶）は、人工知能の種類の一つです。パターンを記憶し、ノイズ入りパターンから元のパターンを思い出す機能を有します。思い出すことに成功するかどうかは、ニューロン数とパターン数、ノイズの割合により決まります。

## このプログラムでできること
本プログラムで Hopfield network の簡単な実験を行えます。画像の記憶、思い出しを行うもので、2つの実行ファイルから成ります：

- `memorize`：画像を記憶し、ニューロンの学習結果を作成
- `recall`：ノイズ入り画像から元の画像を思い出し、その過程を動画で出力

## 実行例
本プログラムの実行例を以下に示します。

1. 記憶する画像を用意

![準備する画像の例](./github.png "github.png")

本プログラムで人間が用意する画像の枚数は1枚です。残りはプログラムがランダムに生成します。画像サイズが大きすぎるとプログラムの実行に時間が掛かります。上のサンプルは 100 x 100 の画像です。もしうまく読み込めない場合は白黒二値画像を使うと良いかもしれません。

2. `memorize` を実行

```bash
./Release/memorize github.png 1000 p1000_github.dat
```

このコマンドにより、1000枚の画像（1枚は `github.png`、999枚はランダム生成）を記憶したニューラルネットワークデータファイル `p1000_github.dat` が出力されます。入力した画像データもこのファイルに格納されます。

3. `recall` を実行

```bash
./Release/recall p1000_github.dat 30 10 p1000_n30_github.avi 1
```

このコマンドにより、AI の思い出し過程の動画 `p1000_n30_github.avi` が出力されます。この霊では、プログラムはニューラルネットワークに 30% ノイズ入りの `github.png` の画像を見せます。ニューラルネットワークは元の画像を思い出そうとします。AI が思い出している画像は時間変化し、10ステップ後に思い出しは打ち切られます。

引数の意味は、`memorize` や `recall` を引数無しで実行することでも確認できます。

4. 出力された動画をみる

https://github.com/skrbcr/toybox/assets/90087433/b0abb059-5cca-4658-b695-9a718e8a0047

[YouTube 版](https://youtu.be/UE2mZNWXd-A)

完璧ではないですが、ノイズが徐々に除去されています。

## ビルド方法

ビルド・実行をするには、[Boost C++ Libraries](https://www.boost.org/)、[zlib](https://www.zlib.net/) と [OpenCV](https://opencv.org/) をインストールする必要があります。OpenCV は FFmpeg と共にインストールする必要があるかもしれません。

```bash
cmake -DCMAKE_BUILD_TYPE=Release -S . -B Release
cmake --build Release
```

実行速度を上げるため、release ビルドする方が良いです。

これにて、`./Release/memorize` と `./Release/recall` を実行できるようになりました。

