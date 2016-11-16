#pragma once
#include<opencv2\core.hpp>
#include<stdlib.h>
#include"Markup.h"
#define NumOfFeatures 610
#define NumOfStages 20
#define ActiveStages 20
#define Ysize 30
#define Xsize 28
#define Ps 0.5
#define HDIM 32
#define BT_factor 0.5;
using namespace cv;

class VJHaar
{
	unsigned int split(const std::wstring &txt, std::vector<std::wstring> &strs, char ch)
	{
		unsigned int pos = txt.find(ch);
		unsigned int initialPos = 0;
		strs.clear();
		strs.push_back(txt);
		// Decompose statement
		while (pos < txt.length()) {
			if((pos-initialPos)>0)
				strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;
			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, txt.length()-initialPos));

		return strs.size();
	}

	float get(CMarkup &xml, int num = 0);

	char recnum[NumOfFeatures+10];
	int reccompressed[NumOfFeatures + 10][3];

	int startIdx[NumOfStages + 1];
	int recIds[NumOfFeatures+100]; //recId threshold leaves
	float thresholds[NumOfFeatures + 100];
	float leaves[NumOfFeatures + 100][2];
	float stageThresh[NumOfStages+10]; //[NumOfStages]
	int stageNum=NumOfStages; //[1]

	int **integral; //[target.heigth][target.width]
	int **skinint;

	int cols,rows;

	uchar charmap[HDIM][HDIM][HDIM];
public:
	float Pf=0.25, Df=0.25, wB=1; //(Pf+Df)*(wB*Bayes)+wR*(Grey)
	VJHaar();
	~VJHaar();
	VJHaar(String XMLname);
	std::vector<Rect> doTheShit(float scale, int num, int step, float jombogo, float threshold);
	Mat VJHaar::integrator(Mat target);
};

