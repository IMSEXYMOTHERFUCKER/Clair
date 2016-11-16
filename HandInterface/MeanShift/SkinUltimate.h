#pragma once
#include<string>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<fstream>
#define TREE 0
#define PROB 1
#define BAYES 2
#define BAYES_TREE 3
#define GRAY_TREE 4
#define GRAY_PROB 5
#define ALL 6
#define DIM 32
#define Ps 0.5
#define BT_factor 0.5
using namespace std;
class SkinUltimate
{
	float skinMap[DIM][DIM][DIM];
	int numberS[DIM][DIM][DIM];
	int numberN[DIM][DIM][DIM];
public:
	uchar map[4][DIM][DIM][DIM];
	SkinUltimate();
	~SkinUltimate();

	SkinUltimate(string posPath, string negPath);
	cv::Mat doIt(cv::Mat target, int type = 6);
	uchar mapDriver(int type, int b, int g, int r);
};

