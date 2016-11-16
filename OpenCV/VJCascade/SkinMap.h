#pragma once
#include<opencv2\core.hpp>
#include<stdlib.h>
#include<iostream>
#include<math.h>
#include<fstream>
#define Ps 0.5
using namespace cv;
class SkinMap
{
	float norms[32][32][32];
	float normn[32][32][32];
	int numbers[32][32][32];
	int numbern[32][32][32];
	float skinmap[32][32][32];
	uchar charmap[32][32][32];
	//float hsvtable[32][32][32][3];
	//float yrbtable[32][32][32][3];
public:
	/*int cols, rows;
	int integral[480][640];*/
	SkinMap();
	~SkinMap();
	SkinMap(String skinHist,String nonsHist);
	cv::Mat masker(cv::Mat target);
	std::vector<cv::Rect> finder(double threshold, double scale, int num, int **integral, int step);
};

