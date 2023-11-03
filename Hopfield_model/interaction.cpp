#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "ezlib.hpp"

#define sgn(x) (x >= 0 ? 1 : -1)

using namespace std;
namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: interaction [PATTEN 0 FILE] [PATTEN] [OUTPUT FILE]" << endl;
	return 0;
    }

    // pattern 0 の画像の読み込み
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (!image.data) {
        cerr << "[Error] Cannot open \"" << argv[1] << "\"" << endl;
        return -1;
    }
    const int n = image.rows * image.cols;
    const int p = stoi(argv[2]);
    const string strFile = argv[3];

    mt19937 engine(12345);

    ublas::matrix<double> J(n, n);
    vector<ublas::vector<int>> xi(p, ublas::vector<int>(n));

    for (int i = 0; i < image.rows; ++i) for (int j = 0; j < image.cols; ++j) {
        xi[0](i * image.rows + j) = ((int)image.at<unsigned char>(i, j) == 0 ? -1 : 1);
    }
    cout << "[Status] Converted pattern_0.png to pattern vector" << endl;

    // 記憶パターンの生成（乱数）
    for (int mu = 0; mu < p; ++mu) {
        for (int i = 0; i < n; ++i) {
            if (mu != 0) xi[mu](i) = sgn((int)engine());
            for (int j = 0; j <= i; ++j) {
                J(i, j) += xi[mu][i] * xi[mu][j];
            }
        }
    }
    J /= (double)n;
    for (int i = 0; i < n; ++i) for (int j = 0; j < i; ++j) J(j, i) = J(i, j);
    cout << "[Status] Generated random pattern vectors" << endl;

    // interaction file の圧縮書き出し
    if (!skrbcr::ezout(strFile, J.data().begin(), J.size1() * J.size2() * sizeof(double))) {
        cout << "[Error] Cannot output interaction file" << endl;
	return -1;
    }

    cout << "[Status] Succeeded to compress and output interaction file" << endl;
    cout << "[Status] All jobs completed. End this program" << endl;

    return 0;
}
