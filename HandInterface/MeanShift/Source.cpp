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
Mat original;
Rect window1,window2;
bool tracking = false, printthresh = false;
SkinUltimate convert = SkinUltimate("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
uchar map[32][32][32];
char thresholds = -1;

void cvtLikeBoss(Mat &target, Mat &out) {
	uchar *ou = out.data;
	uchar *in = target.data;
	int valid = 0, total = 0, sum = 0, sumskin=0;
	int cols = 3*target.cols;
	int rows = target.rows;
	int lol = 0, olo = 0;
	for (int i = 1; i < rows; i++) {
		lol = target.step*i;
		olo = out.step*i;
		int xlimit = cols + lol;
		for (; lol < xlimit; olo++) {
			int b = in[lol++], g = in[lol++], r = in[lol++];
			sum += b*0.1 + g*0.6 + r*0.3;
			sumskin += map[b >> 3][g >> 3][r >> 3];
			total++;
			if (map[b >> 3][g >> 3][r >> 3] > thresholds) {
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
	//if (printthresh)
	//	std::cout << "Thresh: " << (int)thresholds << " ";
	//else
	//	std::cout << "AccRate: " << (valid*100/total) << " Light: "<< sum/total <<" Skin: "<< sumskin/total;
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
		window1 = Rect(cx0, cy0, cx1 - cx0, cy1 - cy0);
		window2 = window1;
		frame(window1).copyTo(original);
		hue = Mat(hue, window1);
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
		cv::imshow("hist", histImage);
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
	String names[14] = { "0-0","32-0","32-32","64-0","64-32","64-64","96-0","96-32","96-64","128-0","128-32","128-64","128-96","160-96" };
	VideoCapture cap = VideoCapture("../../Datas/TestHand/" + names[1] + ".mp4");
	cap >> frame;
	cv::imshow("window", frame);
	setMouseCallback("window", mouse);
	float reallynigga[] = { 0,255 };
	const float *range = { reallynigga };
	int sum = 0,num=0;
	cv::waitKey(0);
	thresholds = 127;
	int totalframe = cap.get(CAP_PROP_FRAME_COUNT);
	while (true) {
		cap >> frame;
		int diffs = 0;
	again:
		if (frame.empty()) break;
		char c = cv::waitKey(1);
		if (c > 0) { if (c == '+') thresholds++; else if (c == '-') thresholds--; }
		Mat hsv = Mat::zeros(frame.size(), CV_8UC1);
		if (tracking) {
			int tmp = thresholds;
			thresholds = -1;
			Mat dst, hsv, hist1;
			Mat hue = Mat::zeros(frame.size(), CV_8UC1);
			cvtLikeBoss(frame, hue);
			calcBackProject(&hue, 1, 0, hist, dst, &range);
			meanShift(dst, window1, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

			thresholds = tmp;
			printthresh = true;
			int histSize = 255;
			float hranges[] = { 10,255 };
			const float* phranges = hranges;
			Mat skinOfOrig = Mat::zeros(original.size(), CV_8UC1);
			cvtLikeBoss(original, skinOfOrig);
			calcHist(&skinOfOrig, 1, 0, Mat(), hist1, 1, &histSize, &phranges);

			printthresh = false;
			cvtLikeBoss(frame, hue);
			calcBackProject(&hue, 1, 0, hist1, dst, &range);
			meanShift(dst, window2, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
			clock_t end = clock();
			rectangle(frame, window2, Scalar(255, 0, 0), 4);
			rectangle(frame, window1, Scalar(0, 255, 0));
			float errX = std::abs(((float)(window1.x - window2.x)) / window1.width);
			float errY = std::abs(((float)(window1.y - window2.y)) / window1.height);
			if ((errX > 0.01) || (errY > 0.01)) {
				diffs++;
				if (diffs < (totalframe*0.01)) {
					thresholds--;
					window2 = window1;
					goto again;
				}
			}
			//std::cout << "FAIL"<<errX<<" "<<errY;
			//std::cout << "\n";
		}
		cv::imshow("window", frame);
	}
	std::cout << (int)thresholds << "\n";
	cv::waitKey(0);
}