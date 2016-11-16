#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\video.hpp>
#include<iostream>
#include<fstream>
#define DIM 32
using namespace cv;
const Vec3b white(230, 230, 230);
const Vec3b black(30, 30, 30);
int nonskin[DIM][DIM][DIM];
int skin[DIM][DIM][DIM];

void doTheShit(Mat target) {
	for (int i = 0; i < target.rows; i++) {
		for (int j = 0; j < target.cols; j++) {
			Vec3b bgr = target.at<Vec3b>(i, j);
			int sum = bgr[0] + bgr[1] + bgr[2];
			if ((sum>60)&&(sum<705)) {
				skin[bgr[0]>>3][bgr[1]>>3][bgr[2]>>3] += 1;
			}
			else {
				nonskin[bgr[0] >> 3][bgr[1] >> 3][bgr[2] >> 3] ++;
			}
		}
	}
}

int main() {
	//Initialization
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int k = 0; k < DIM; k++) {
				skin[i][j][k] = 0;
				nonskin[i][j][k] = 0;
			}
		}
	}

		//Positive Samples
		for (int i = 1; i <= 48; i++) {
			std::string sidechick = "Samples/AllSamples/" + std::to_string(i) + ".png";
			doTheShit(imread(sidechick));
		}
		for (int i = 1; i <= 48; i++) {
			std::string sidechick = "Samples/Masked/b (" + std::to_string(i) + ").jpg";
			doTheShit(imread(sidechick));
		}

		std::ifstream t("Samples/Skin_NonSkin.txt");
		std::string strS((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		int b = 0, g = 0, r = 0;
		int curNum = 0, count = 0;
		for each (char lol in strS)
		{
			if (lol == '\t') {
				count++;
				if (count == 1) {
					b = curNum;
					if (b > 255)
						int here = lol;
					curNum = 0;
				}
				else if (count == 2) {
					g = curNum;
					curNum = 0;
				}
				else {
					r = curNum;
					curNum = 0;
				}
			}
			else if (lol == '\n') {
				if (curNum == 1) {
					skin[b >> 3][g >> 3][r >> 3]++;
				}
				else {
					nonskin[b >> 3][g >> 3][r >> 3]++;
				}
				count = 0;
				curNum = 0;
			}
			else {
				curNum = curNum * 10 + (lol - '0');
			}
		}

		std::ofstream fout("skinU1.txt");
		if (fout.is_open())
		{
			//read from stored 1D array to 3D array
			for (int i = 0; i < DIM; i++)
			{
				for (int j = 0; j < DIM; j++)
				{
					for (int k = 0; k < DIM; k++)
					{
						fout << skin[i][j][k];
						fout << ' ';
					}
				}
			}
		}
		fout.close();

		//Negative Samples
		VideoCapture cap1 = VideoCapture("Samples/Russia.MP4");
		VideoCapture cap2 = VideoCapture("Samples/nfs.MP4");
		Mat frame1, frame2;
		int idx = 0;
		int progress = 0;;
		while (true) {
			cap1 >> frame1;
			cap2 >> frame2;
			if (frame2.empty()) break;
			if (idx == 25) {
				progress++;
				std::cout << (progress / 28.16) << "\n";
				idx = 0;
				for (int i = 0; i < frame1.rows; i++) {
					for (int j = 0; j < frame1.cols; j++) {
						Vec3b bgr = frame1.at<Vec3b>(i, j);
						nonskin[bgr[0] >> 3][bgr[1] >> 3][bgr[2] >> 3] += 1;

						bgr = frame2.at<Vec3b>(i, j);
						nonskin[bgr[0] >> 3][bgr[1] >> 3][bgr[2] >> 3] += 1;
					}
				}
			}
			idx++;
		}
		std::ofstream fout1("NHistogram.txt");
		if (fout1.is_open())
		{
			//read from stored 1D array to 3D array
			for (int i = 0; i < DIM; i++)
			{
				for (int j = 0; j < DIM; j++)
				{
					for (int k = 0; k < DIM; k++)
					{
						fout1 << nonskin[i][j][k];
						fout1 << ' ';
					}
				}
			}
		}
		fout1.close();
}