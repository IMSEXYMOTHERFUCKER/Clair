#pragma once
#include<opencv2\core.hpp>
#include<stdlib.h>
#include"Markup.h"
#define NumOfFeatures 2134
#define NumOfStages 22
#define Ps 0.5
#define HDIM 32
#define shit 3
#define BT_factor 0.5;
using namespace cv;

class VJLbp
{
	unsigned int split(const std::wstring &txt, std::vector<std::wstring> &strs, char ch)
	{
		unsigned int pos = txt.find(ch);
		unsigned int initialPos = 0;
		strs.clear();
		strs.push_back(txt);
		// Decompose statement
		while (pos < txt.length()) {
			if ((pos - initialPos)>0)
				strs.push_back(txt.substr(initialPos, pos - initialPos + 1));
			initialPos = pos + 1;
			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, txt.length() - initialPos));

		return strs.size();
	}

	void pointer(int i, int x, int y, int width, int height) {
		lines[i][0][0] = y; lines[i][0][1] = x;
		lines[i][1][0] = y + height; lines[i][1][1] = x + width;
		lines[i][2][0] = y + 2*height; lines[i][2][1] = x + 2*width;
		lines[i][3][0] = y + 3*height; lines[i][3][1] = x + 3*width;
	}

	inline int getSum(int y0, int y1, int x0, int x1) {
		return integral[y0][x0] + integral[y1][x1] - integral[y0][x1] - integral[y1][x0];
	}

	float get(CMarkup &xml, int num = 0);

	float leaves[NumOfFeatures][2]; //[NumOfFeatures]
	int lines[NumOfFeatures][4][2];
	int subset[NumOfFeatures][8];

	int startIdx[NumOfStages + 1]; //[NumOfStages]
	float stageThresh[NumOfStages]; //[NumOfStages]
	int stageNum = NumOfStages; //[1]

	int **integral; //[target.heigth][target.width]
	int **skinint;

	uchar charmap[HDIM][HDIM][HDIM];
public:
	float Pf = 0.25, Df = 0.25, wB = 1; //(Pf+Df)*(wB*Bayes)+wR*(Grey)
	VJLbp();
	~VJLbp();
	VJLbp(String XMLname);
	std::vector<Rect> doTheShit(float scale, int num, int step, float jombogo, float threshold);
	Mat integrator(Mat target);
};

