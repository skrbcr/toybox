#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "ezlib.hpp"

#define sgn(x) (x >= 0 ? 1 : -1)

using namespace std;
namespace ublas = boost::numeric::ublas;

void make_image(cv::Mat &image, ublas::vector<int> &s, int width, int height) {
    int dr = image.rows / width, dc = image.cols / height;
    for (int i = 0; i < width; ++i) for (int j = 0; j < height; ++j) for (int k = 0; k < dr; ++k) for (int l = 0; l < dr; ++l) image.at<unsigned char>(i * dr + k, j * dc + l) = (s[i * height + j] == 1 ? (unsigned char)255 : (unsigned char)0);
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cout << "Usage: movie [INTERACTION FILE] [PATTERN 0 FILE] [NOISE] [STEPS] [OUTPUT MOVIE FILE] [OUTPUT IMAGE]\n"
             << "\tNOISE: noise ratio. 0~100\n"
             << "\tOUTPUT IMAGE: 1 if output image files of each step. 0 if not"
	     << endl;
        return 0;
    }

    const string strInFile = argv[1];

    // pattern 0 の画像の読み込み
    cv::Mat image = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        cerr << "[Error] Cannot find \"" << argv[2] << "\"" << endl;
        return -1;
    }
    const int n = image.rows * image.cols;
    const unsigned long noise = stoul(argv[3]);
    const int duration = stoi(argv[4]);
    const string strOutFile = argv[5];
    bool bOutImage = stoi(argv[6]) == 1 ? true : false;

    mt19937 engine(12345);

    ublas::matrix<double> J(n, n);

    // interaction file の解凍読み込み
    if (!skrbcr::ezin(strInFile, J.data().begin(), J.size1() * J.size2() * sizeof(double))) {
        cout << "[Error] Cannot input interaction file" << endl;
	return -1;
    }

    ublas::vector<int> s(n);
    for (int i = 0; i < image.rows; ++i) for (int j = 0; j < image.cols; ++j) {
        s(i * image.rows + j) = ((int)image.at<unsigned char>(i, j) == 0 ? -1 : 1);
    }
    
    // 画像出力の準備
    const string strOutImgDir = strOutFile + "_img/";
    if (bOutImage) filesystem::create_directory(strOutImgDir);
    cv::Mat imgOut(1000, 1000, CV_8U);

    // 思い出したいパターンを生成
    for (int i = 0; i < n; ++i) {
        if (engine() % 100 <= noise) s[i] = -s[i];
    }
    /* for (int i = 0; i < n; ++i) image.at<unsigned char>(i / image.rows, i % image.rows) = (s[i] == 1 ? (unsigned char)255 : (unsigned char)0); */
    make_image(imgOut, s, image.rows, image.cols);

    cv::VideoWriter output(strOutFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 1., cv::Size(imgOut.rows, imgOut.cols), false);
    if (!output.isOpened()) {
        cerr << "[Error] Cannot open \"" << strOutFile << "\"" << endl;
        return -1;
    }
    output << imgOut;
    if (bOutImage) cv::imwrite(strOutImgDir + "0.png", imgOut);

    // 思い出し
    ublas::vector<double> stemp(n);
    for (int t = 1; t < duration; ++t) {
        stemp = ublas::prod(J, s);
        for (int i = 0; i < n; ++i) s(i) = sgn(stemp(i));
        make_image(imgOut, s, image.rows, image.cols);
        output << imgOut;
        if (bOutImage) cv::imwrite(strOutImgDir + to_string(t) + ".png", imgOut);
    }
    output.release();

    return 0;
}
