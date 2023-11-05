#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <zlib.h>
#include "Hopfield_model.hpp"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define sgn(x) (x >= 0 ? 1 : -1)

using namespace std;
namespace ublas = boost::numeric::ublas;

int main(int argc, char* argv[]) {
	if (argc != 4) {
		cout << "Usage: memorize [PATTEN 0 FILE] [PATTEN] [OUTPUT FILE]" << endl;
	return 0;
	}

	// Load image of pattern 0
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
	cout << "[Status] Converted \"" << argv[1] << "\" to pattern vector" << endl;

	// Generate random patterns
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

	// Compress and output
	ofstream ofs(strFile, ios::out | ios::binary);
	if (!ofs) {
	cerr << "[Error] Cannot open \"" << strFile << "\"" << endl;
	return -1;
	}
	ofs << SKRBCR_HOPFILED_MODEL_DATA_FILE_HEADER << '\n'
		<< SKRBCR_HOPFIELD_MODEL_VERSION << '\n'
		<< image.rows << " " << image.cols << endl;
	z_stream zs;
	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;
	if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
		cerr << "[Error] Failed to init compressing file" << endl;
		ofs.close();
		return -1;
	}
	zs.next_in = reinterpret_cast<Bytef*>(xi[0].data().begin());
	zs.avail_in = xi[0].size() * sizeof(int);
	unsigned char outbuf[262144];
	do {
		zs.next_out = outbuf;
		zs.avail_out = sizeof(outbuf);
		if (deflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
			deflateEnd(&zs);
			ofs.close();
			cerr << "[Error] Something went wrong with compressing data" << endl;
			return -1;
		}
		ofs.write(reinterpret_cast<char*>(outbuf), sizeof(outbuf) - zs.avail_out);
	} while (zs.avail_out == 0);
	ofs << endl;
	if (deflateReset(&zs) != Z_OK) {
		cerr << "[Error] Something went wrong with compressing data" << endl;
		return -1;
	}

	if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
		cerr << "[Error] Failed to init compressing file: " << endl;
		ofs.close();
		return -1;
	}
	zs.next_in = reinterpret_cast<Bytef*>(J.data().begin());
	zs.avail_in = J.size1() * J.size2() * sizeof(double);
	do {
		zs.next_out = outbuf;
		zs.avail_out = sizeof(outbuf);
		if (deflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
			deflateEnd(&zs);
			ofs.close();
			cerr << "[Error] Something went wrong with compressing data" << endl;
			return -1;
		}
		ofs.write(reinterpret_cast<char*>(outbuf), sizeof(outbuf) - zs.avail_out);
	} while (zs.avail_out == 0);
	ofs << endl;
	if (deflateEnd(&zs) != Z_OK) {
		cerr << "[Error] Something went wrong with compressing data" << endl;
		return -1;
	}

	cout << "[Status] Succeeded to compress and output interaction file" << endl;
	cout << "[Status] All jobs completed. End this program" << endl;

	return 0;
}
