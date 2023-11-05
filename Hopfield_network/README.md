# Hopfield network

日本語の説明は[こちら](./README_ja.md)です。

## What is Hopfield network
Hopfield network, a.k.a. associative memory is a kind of artificial intelligence. This neural network can memorize data patterns and when one pattern with noise is given, it may recall the original pattern. Whether the network succeeds to recall is depends on the number of neurons, the number of patterns and noise ratio.

## What this program can do
With this program, you can conduct a small expriment. It memorize and recall images and consists of two executable file:

- `memorize`: Memorize images and make trained neural network data
- `recall`: Recall image and create a video of the process

### Example & How to use
One example of result is shown below.

1. Prepare a image that the AI memorize

![Prepared image example](./github.png "github.png")

You have to prepare only one image. The rest images are generated automatically and randomly. If the size of the image is too large, it takes long time to run. The size of the above image is 100 x 100. If the program occures error with reading image, you may use binary image.

2. Run `memorize`

```bash
./Release/memorize github.png 1000 p1000_github.dat
```

This command create trained neural network data file `p1000_github.dat` with 1000 images (1 is `github.png` and 999 are random generated images) memorized. The image file you inputted is also saved in this file.

3. Run `recall`

```bash
./Release/recall p1000_github.dat 30 10 p1000_n30_github.avi 1
```

This command create a video `p1000_n30_github.avi` that shows the process of recalling. In this example, the program gives the neural network `github.png` with 30% noise. The neural network try to recall original image `github.png`. The image that the AI is recalling changes time by time and 10 steps after, the program stops the AI.

You can learn the meaning of the args by running `memorize` and `recall` without args.

4. Watch the output video

https://github.com/skrbcr/toybox/assets/90087433/b0abb059-5cca-4658-b695-9a718e8a0047

[YouTube version](https://youtu.be/UE2mZNWXd-A)

Not perfectly, but noise is removed gradually.

## How to build

To build and run, you have to install [Boost C++ Libraries](https://www.boost.org/) and [OpenCV](https://opencv.org/). You may Install OpenCV with FFmpeg.

```bash
cmake -DCMAKE_BUILD_TYPE=Release -S . -B Release
cmake --build Release
```

To run the programs fast, you may choose release build.

Then, you can run `./Release/memorize` and `./Release/recall`.

