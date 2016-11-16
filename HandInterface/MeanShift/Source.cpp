#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\video.hpp>
#include<iostream>
#include<time.h>
#include"SkinUltimate.h"
#include<math.h>
using namespace cv;

int cx0=0,cy0=0,cx1=0,cy1=0;
Mat frame;
Mat hist;
Rect window;
bool tracking = false;
SkinUltimate convert = SkinUltimate("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
uchar map[32][32][32];
char thresholds = 60;

void cvtLikeBoss(Mat &target, Mat &out) {
	uchar *ou = out.data;
	uchar *in = target.data;

	int cols = 3*target.cols;
	int rows = target.rows;
	int lol = 0, olo = 0;
	for (int i = 1; i < rows; i++) {
		lol = target.step*i;
		olo = out.step*i;
		int xlimit = cols + lol;
		for (; lol < xlimit; olo++) {
			int b = in[lol++], g = in[lol++], r = in[lol++];
			if (map[b >> 3][g >> 3][r >> 3] > thresholds) {
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
		cx0 = x; cy0 = y;
	}
	else if (event == EVENT_LBUTTONUP) {
		cx1 = x; cy1 = y;
	}
	else if (event == EVENT_RBUTTONDOWN) {
		tracking = !tracking;
		Mat hsv;// = convert.doIt(frame, 2);
		int histSize = 255;
		float hranges[] = { 10,255 };
		const float* phranges = hranges;
		Mat hue = Mat::zeros(frame.size(), CV_8UC1);
		cvtLikeBoss(frame, hue);
		window = Rect(cx0, cy0, cx1 - cx0, cy1 - cy0);
		hue = Mat(hue, window);
		calcHist(&hue, 1, 0, Mat(), hist, 1, &histSize, &phranges);

		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);
		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
		Mat nhist;
		normalize(hist, nhist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(nhist.at<float>(i - 1))),
				Point(bin_w*(i), hist_h - cvRound(nhist.at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
		}
		imshow("hist", histImage);
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
	float reallynigga[] = { 0,255 };
	const float *range = { reallynigga };
	int sum = 0,num=0;
	while (true) {
		cap >> frame;
		waitKey(1);
		Mat hsv = Mat::zeros(frame.size(), CV_8UC1);
		if (tracking) {
			clock_t start=clock();
			Mat dst, hsv;
			Mat hue = Mat::zeros(frame.size(), CV_8UC1);
			cvtLikeBoss(frame, hue);
			imshow("hue", hue);
			calcBackProject(&hue, 1, 0, hist, dst, &range);
			meanShift(dst, window, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
			rectangle(frame, window, 255);
			imshow("dst", dst);
			clock_t end=clock();
			std::cout << end - start << "\n";
		}
		imshow("window", frame);
	}
}