#pragma once
#include<opencv2\core.hpp>
#include<stdlib.h>
#include<math.h>
using namespace cv;
class SkinMaskClassifier2
{
public:
	/*int cols, rows;
	int integral[480][640];*/
	SkinMaskClassifier2();
	~SkinMaskClassifier2();
	cv::Mat masker(cv::Mat target, int **integral);
	std::vector<cv::Rect> finder(double threshold, double scale, int num, int **integral, int step);
};

