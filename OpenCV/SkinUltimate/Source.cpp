#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\video.hpp>
#include<string>
#include<iostream>
#include"SkinUltimate.h"
using namespace cv;
int main() {
	SkinUltimate ultimate = SkinUltimate("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
	int type=3;
	Mat frame;

	/*for (int i = 1; i <= 3285; i++) {
		frame = imread("Negatives/n (" + to_string(i) + ").jpg");
		imwrite("Negatives/Bayes/n (" + to_string(i+2134) + ").jpg", ultimate.doIt(frame, 2));
		imwrite("Negatives/BayesTree/n (" + to_string(i+2134) + ").jpg", ultimate.doIt(frame, 3));
		std::cout << i << "\n";
	}*/

	VideoCapture cap = VideoCapture(0);
	while (true) {
		char c = waitKey(1);
		if (c > -1) {
			type = c - '0';
		}
		cap >> frame;
		imshow("Hehe", ultimate.doIt(frame, type));
	}
}