#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include"../../OpenCV/SkinUltimate/SkinUltimate.h"
#include<iostream>
using namespace cv;

Mat frame,hist;
Rect window=Rect(0,0,0,0);
bool ok = false;
SkinUltimate convert = SkinUltimate("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
uchar map[32][32][32];
uchar thresholds = 125;

void cvtLikeBoss(Mat &target, Mat &out) {
	uchar *ou = out.data;
	uchar *in = target.data;
	int valid = 0, total = 0, sum = 0, sumskin = 0;
	int cols = 3 * target.cols;
	int rows = target.rows;
	int lol = 0, olo = 0;
	for (int i = 1; i < rows; i++) {
		lol = target.step*i;
		olo = out.step*i;
		int xlimit = cols + lol;
		for (; lol < xlimit; olo++) {
			int b = in[lol++], g = in[lol++], r = in[lol++];
			total++;
			if (map[b >> 3][g >> 3][r >> 3] >= thresholds) {
				valid++;
				//do the hsv
				int max = g, min = g;
				if (r > b) {
					if (r > g) {
						max = r;
						if (g > b) min = b;
					}
					else { min = b; }
				}
				else {
					if (b > g) {
						max = b;
						if (g > r) min = r;
					}
					else { min = r; }
				}
				int delta = max - min;
				if (delta > 0) {
					float hue;
					if (r == max) {
						hue = 4 + (float)(g - b) / delta;
					}
					else {
						if (g == max) {
							hue = 0 + (float)(b - r) / delta;
						}
						else {
							hue = 2 + (float)(r - g) / delta;
						}
					}
					if (hue < 0) hue += 6;
					ou[olo] = hue * 46;
				}
			}
		}
	}
}

void mouse(int event, int x, int y, int flags, void* h) {
	if (event == EVENT_LBUTTONDOWN) {
		window.x = x;
		window.y = y;
	}
	else if (event == EVENT_LBUTTONUP) {
		window.width = x - window.x;
		window.height = y - window.y;
	}
	else if (event == EVENT_RBUTTONDOWN) {
		int histSize = 255;
		float hranges[] = { 10,255 };
		const float* phranges = hranges;
		frame = Mat(frame, window);
		Mat hue = Mat::zeros(frame.size(), CV_8UC1);
		cvtLikeBoss(frame, hue);
		calcHist(&hue, 1, 0, Mat(), hist, 1, &histSize, &phranges);
		ok = !ok;
	}
}


int main() {
	for (int b = 0; b < 32; b++) {
		for (int g = 0; g < 32; g++) {
			for (int r = 0; r < 32; r++) {
				map[b][g][r] = convert.map[BAYES_TREE][b][g][r];
			}
		}
	}
	VideoCapture cap = VideoCapture(0);
	cap >> frame;
	cv::imshow("window", frame);
	setMouseCallback("window", mouse);
	float reallynigga[] = { 10,255 };
	const float *range = { reallynigga };
	while (1) {
		char c = waitKey(1);
		if (c > 0) { if (c == '+') thresholds++; else if (c == '-') thresholds--; }
		cap >> frame;
		Mat hue = Mat::zeros(frame.size(), CV_8UC1), dst;
		cvtLikeBoss(frame, hue);
		if (ok) {
			calcBackProject(&hue, 1, 0, hist, dst, &range);
			threshold(dst, dst, 10, 255, THRESH_BINARY);
			rectangle(dst, window, 255);
			imshow("dst", dst);
		}
		imshow("hue", hue);
		imshow("window", frame);
		std::cout << (int)thresholds << "\n";
	}
}