#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <filesystem>
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

void make_image(cv::Mat &image, ublas::vector<int> &s, int width, int height) {
	int dr = image.rows / width, dc = image.cols / height;
	for (int i = 0; i < width; ++i) for (int j = 0; j < height; ++j) for (int k = 0; k < dr; ++k) for (int l = 0; l < dr; ++l) image.at<unsigned char>(i * dr + k, j * dc + l) = (s[i * height + j] == 1 ? (unsigned char)255 : (unsigned char)0);
}

void read_line(ifstream &ifs, string &str) {
	str = "";
	char szT[64];
	while (true) {
		ifs.getline(szT, sizeof(szT));
		str += szT;
		if (ifs.fail()) ifs.clear(ifs.rdstate() & ~ios_base::failbit);
		else break;
	}
}

int main(int argc, char* argv[]) {
	if (argc != 6) {
		cout << "Usage: recall [DATA FILE] [NOISE] [STEPS] [OUTPUT MOVIE FILE] [OUTPUT IMAGE]\n"
			 << "\tNOISE: noise ratio. 0~100\n"
			 << "\tOUTPUT IMAGE: 1 if output image files of each step. Else if not"
		 << endl;
		return 0;
	}

	const string strInFile = argv[1];

	// Load and decompress data file
	ifstream ifs(argv[1], ios::in | ios::binary);
	if (!ifs) {
		cerr << "[Error] Cannot open\"" << argv[1] << "\"" << endl;
	return -1;
	}
	string strT;
	read_line(ifs, strT);
	if (strT != SKRBCR_HOPFILED_MODEL_DATA_FILE_HEADER) {
		cerr << "[Error] Input file is not Hopfield model data file" << endl;
	return -1;
	}
	read_line(ifs, strT);
	if (strT != SKRBCR_HOPFIELD_MODEL_VERSION) {
		cerr << "[Error] Version of input data file is" << strT << " but the version of program is" << SKRBCR_HOPFIELD_MODEL_VERSION << endl;
	return -1;
	}
	read_line(ifs, strT);
	istringstream iss(strT);
	int width, height;
	iss >> width >> height;
	int g_header = ifs.tellg();

	const int n = width * height;
	const unsigned long noise = stoul(argv[2]);
	const int duration = stoi(argv[3]);
	const string strOutFile = argv[4];
	bool bOutImage = stoi(argv[5]) == 1 ? true : false;

	mt19937 engine(12345);

	ublas::matrix<double> J(n, n);
	ublas::vector<int> s(n);

	z_stream zs;
	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;
	if (inflateInit(&zs) != Z_OK) {
		cerr << "[Error] Failed to init depressing file" << endl;
		ifs.close();
		return -1;
	}
	zs.avail_out = s.size() * sizeof(int);
	zs.next_out = reinterpret_cast<Bytef*>(s.data().begin());
	unsigned char inbuf[262144];
	int last_in;
	do {
		ifs.read(reinterpret_cast<char*>(inbuf), sizeof(inbuf));
		zs.avail_in = ifs.gcount();
		zs.next_in = inbuf;
		last_in = zs.avail_in;
		if (inflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
			inflateEnd(&zs);
			ifs.close();
			cerr << "[Error] Something went wrong with decompressing data" << endl;
			return -1;
		}
	} while (zs.avail_in == 0);
	if (!ifs) {
		ifs.close();
		ifs.open(argv[1]);
		if (!ifs) {
			cerr << "[Error] Cannot open\"" << argv[1] << "\"" << endl;
			return -1;
		}
	}
	ifs.seekg(g_header + last_in - zs.avail_in + 1, ios::beg);
	if (inflateReset(&zs) != Z_OK) {
		ifs.close();
		cerr << "[Error] Something went wrong with decompressing data" << endl;
		return -1;
	}

	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;
	if (inflateInit(&zs) != Z_OK) {
		cerr << "[Error] Failed to init depressing file" << endl;
		ifs.close();
		return -1;
	}
	zs.avail_out = J.size1() * J.size2() * sizeof(double);
	zs.next_out = reinterpret_cast<Bytef*>(J.data().begin());
	do {
		ifs.read(reinterpret_cast<char*>(inbuf), sizeof(inbuf));
		zs.avail_in = ifs.gcount();
		zs.next_in = inbuf;
		if (inflate(&zs, Z_FINISH) == Z_STREAM_ERROR) {
			inflateEnd(&zs);
			ifs.close();
			cerr << "[Error] Something went wrong with decompressing data" << endl;
			return -1;
		}
	} while (zs.avail_in == 0);
	if (inflateEnd(&zs) != Z_OK) {
		ifs.close();
		cerr << "[Error] Something went wrong with decompressing data" << endl;
		return -1;
	}
	ifs.close();
	cout << "[Status] Decompress input file" << endl;
	
	// Prepare outputting image if needed
	const string strOutImgDir = strOutFile + "_img/";
	if (bOutImage) filesystem::create_directory(strOutImgDir);
	cv::Mat imgOut(1000, 1000, CV_8U);

	// Create pattern with noise
	for (int i = 0; i < n; ++i) if (engine() % 100 <= noise) s[i] = -s[i];
	make_image(imgOut, s, width, height);

	cv::VideoWriter output(strOutFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 1., cv::Size(imgOut.rows, imgOut.cols), false);
	if (!output.isOpened()) {
		cerr << "[Error] Cannot open \"" << strOutFile << "\"" << endl;
		return -1;
	}
	output << imgOut;
	if (bOutImage) cv::imwrite(strOutImgDir + "0.png", imgOut);

	// Retrieving
	ublas::vector<double> stemp(n);
	for (int t = 1; t < duration; ++t) {
		stemp = ublas::prod(J, s);
		for (int i = 0; i < n; ++i) s(i) = sgn(stemp(i));
		make_image(imgOut, s, width, height);
		output << imgOut;
		if (bOutImage) cv::imwrite(strOutImgDir + to_string(t) + ".png", imgOut);
	}

	cout << "[Status] Succeeded to output video" << endl;
	cout << "[Status] All jobs completed. End this program" << endl;

	return 0;
}
