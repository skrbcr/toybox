# Hopfield network

## Hopfield network とは
Hopfield network（ホップフィールド・ネットワーク）またの名を associative memory（連想記憶）は、人工知能の種類の一つです。パターンを記憶し、ノイズ入りパターンから元のパターンを思い出す機能を有します。思い出すことに成功するかどうかは、ニューロン数とパターン数、ノイズの割合により決まります。

## このプログラムでできること
本プログラムで Hopfield network の簡単な実験を行えます。画像の記憶、思い出しを行うもので、2つの実行ファイルから成ります：

- `interaction`：画像を記憶し、ニューロンの学習結果を作成
- `movie`：ノイズ入り画像から元の画像を思い出し、その過程を動画で出力

## 実行例
本プログラムの実行例を以下に示します。

1. 記憶する画像を用意

![Prepared image example](./github.png "github.png")

本プログラムで人間が用意する画像の枚数は1枚です。残りはプログラムがランダムに生成します。画像サイズが大きすぎるとプログラムの実行に時間が掛かります。上のサンプルは 100x100 の画像です。もしうまく読み込めない場合は白黒二値画像を使うと良いかもしれません。

2. `interaction` を実行

```bash
./Release/interaction github.png 1000 p1000_github.int
```

このコマンドにより、1000枚の画像（1枚は `github.png`、999枚はランダム生成）を記憶したニューラルネットワークデータファイル `p1000_github.int` が出力されます。

3. `movie` を実行

```bash
./Release/movie p1000_github.int github.png 30 10 p1000_n30_github.avi 1
```

このコマンドにより、AI の思い出し過程の動画 `p1000_n30_github.avi` が出力されます。プログラムはニューラルネットワークに、30% ノイズ入りの `github.png` の画像を見せます。ニューラルネットワークは元の画像を思い出そうとします。AI が思い出している画像は時間変化し、10ステップ後に思い出しは打ち切られます。

引数の意味は、`interaction` や `movie` を引数無しで実行することでも確認できます。

4. 出力された動画をみる

<iframe width="560" height="315" src="https://www.youtube.com/embed/UE2mZNWXd-A?si=UPaRYwNLRRhLeZW9" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

ノイズが徐々に除去されています。

## ビルド方法

ビルド・実行をするには、[Boost C++ Libraries](https://www.boost.org/) と [OpenCV](https://opencv.org/) をインストールする必要があります。OpenCV は FFmpeg と共にインストールする必要があるかもしれません。

```bash
cmake -DCMAKE_BUILD_TYPE=Release -S . -B Release
cmake --build Release
```

実行速度を上げるため、release ビルドする方が良いです。

これにて、`./Release/interaction` と `./Release/movie` を実行できるようになりました。

