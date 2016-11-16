#include "VJLbp.h"
#include <opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
#include<fstream>
#include<iostream>

float VJLbp::get(CMarkup & xml, int num)
{
	std::wstring lol = xml.GetData();
	std::vector<std::wstring> v;
	split(lol, v, ' ');
	return std::stof(v[num]);
}

VJLbp::VJLbp()
{
}

VJLbp::~VJLbp()
{
}

VJLbp::VJLbp(String XMLname)
{
	int numbers[HDIM][HDIM][HDIM];
	int numbern[HDIM][HDIM][HDIM];
	float skinmap[HDIM][HDIM][HDIM];
	int lutrec[NumOfFeatures];

	std::ifstream t("skinU1.txt");
	std::string strS((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::ifstream z("NHistogram.txt");
	std::string strN((std::istreambuf_iterator<char>(z)), std::istreambuf_iterator<char>());
	int b = 0, g = 0, r = 0;
	double sums = 0, sumn = 0;

	//get numbers
	for (b = 0; b < HDIM; b++) {
		for (g = 0; g < HDIM; g++) {
			for (r = 0; r < HDIM; r++) {
				numbers[b][g][r] = 0;
				numbern[b][g][r] = 0;
				skinmap[b][g][r] = 0;
				charmap[b][g][r] = 0;
			}
		}
	}
	int max = 0;
	b = 0; g = 0; r = 0;
	for each (char lol in strS)
	{
		if (lol == ' ') {
			if (numbers[b][g][r] > max) max = numbers[b][g][r];
			sums += numbers[b][g][r];
			if (r == HDIM - 1) {
				r = 0;
				if (g == HDIM - 1) {
					g = 0;
					if (b == HDIM - 1) {

					}
					else { b++; }
				}
				else { g++; }
			}
			else { r++; }
		}
		else {
			numbers[b][g][r] = numbers[b][g][r] * 10 + (lol - '0');
		}
	}
	b = 0; g = 0; r = 0;
	for each (char lol in strN)
	{
		if (lol == ' ') {
			sumn += numbern[b][g][r];
			if (r == HDIM - 1) {
				r = 0;
				if (g == HDIM - 1) {
					g = 0;
					if (b == HDIM - 1) {

					}
					else { b++; }
				}
				else { g++; }
			}
			else { r++; }
		}
		else {
			numbern[b][g][r] = numbern[b][g][r] * 10 + (lol - '0');
		}
	}

	float maxskin = 0;
	float boom3 = sums / sumn;
	for (b = 0; b < HDIM; b++) {
		for (g = 0; g < HDIM; g++) {
			for (r = 0; r < HDIM; r++) {
				if (numbers[b][g][r] != 0) {
					int boom1 = numbers[b][g][r], boom2 = numbern[b][g][r];
					skinmap[b][g][r] = (Ps*boom1) / (Ps*boom1 + (1 - Ps) * boom2 * boom3);	//Ps_c = (Pc_s*Ps) / (Pc_s*Ps + Pc_ns(1-Ps));
					if (skinmap[b][g][r] > maxskin) maxskin == skinmap[b][g][r];
				}
			}
		}
	}

	for (b = 0; b < HDIM; b++) {
		for (g = 0; g < HDIM; g++) {
			for (r = 0; r < HDIM; r++) {
				//charmap[b][g][r] = (numbers[b][g][r] * 255 / max);
				//charmap[b][g][r] = ((skinmap[b][g][r]) * 255);
				//if (numbers[b][g][r] > (boom3*numbern[b][g][r])) 
				//	charmap[b][g][r] = skinmap[b][g][r] * 255;
				//else
				//	charmap[b][g][r] = skinmap[b][g][r] * 255 * BT_factor;
				if (((r << 3) > 95) && ((g << 3) > 40) && ((b << 3) > 20) && ((r << 3) > (b << 3)) && ((r << 3) > (g << 3) + 15) && ((r << 3) > min((b << 3), (g << 3)) + 15)) {
					charmap[b][g][r] = skinmap[b][g][r] * 255;
				}
				else {
					charmap[b][g][r] = skinmap[b][g][r] * 255 * BT_factor;
				}
				//if (((double)numbers[b][g][r] / sums) > ((double)numbern[b][g][r] / sumn))
				//	charmap[b][g][r] = 255;
				//else
				//	charmap[b][g][r] = 0;
			}
		}
	}

	integral = new int*[480];
	for (int i = 0; i < 480; ++i)
		integral[i] = new int[640];
	skinint = new int*[480];
	for (int i = 0; i < 480; ++i)
		skinint[i] = new int[640];
	CMarkup xml;
	xml.Load(MCD_T("facelbp.xml"));
	startIdx[0] = 0;
	xml.FindElem(MCD_T("opencv_storage"));
	xml.IntoElem();
	xml.FindElem(MCD_T("cascade"));
	xml.IntoElem();
	//xml.FindElem(MCD_T("stagesNum"));
	//stageNum = get();
	xml.FindElem(MCD_T("stages"));
	xml.IntoElem();
	int weakI = 0;
	for (int stageI = 0; stageI<stageNum; stageI++) {
		xml.FindElem(MCD_T("_"));
		xml.IntoElem();
		xml.FindElem(MCD_T("maxWeakCount"));
		startIdx[stageI + 1] = startIdx[stageI] + get(xml);
		xml.FindElem(MCD_T("stageThreshold"));
		stageThresh[stageI] = get(xml);
		xml.FindElem(MCD_T("weakClassifiers"));
		xml.IntoElem();
		for (; weakI<startIdx[stageI + 1]; weakI++) {
			xml.FindElem(MCD_T("_"));
			xml.IntoElem();
			bool joom = xml.FindElem(MCD_T("internalNodes"));
			lutrec[(int)get(xml, 3 + 1)] = weakI;
			for (int lol = 0; lol<8; lol++) {
				subset[weakI][0] = get(xml, lol + 4 + 1);
			}
			xml.FindElem(MCD_T("leafValues"));
			leaves[weakI][0] = get(xml, 1 + 1);
			leaves[weakI][1] = get(xml, 2 + 1);
			xml.OutOfElem();
		}
		xml.OutOfElem();
		xml.OutOfElem();
	}

	xml.OutOfElem();
	xml.FindElem(MCD_T("features"));
	xml.IntoElem();
	for (int i = 0; i<weakI; i++) {
		xml.FindElem(MCD_T("_"));
		xml.IntoElem();
		xml.FindElem(MCD_T("rects"));
		xml.IntoElem();
			int x = get(xml, 1 + 1);
			int y = get(xml, 2 + 1);
			int width = get(xml, 3 + 1);
			int height = get(xml, 4 + 1);
			pointer(lutrec[i], x, y, width, height);
		xml.OutOfElem();
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.OutOfElem();
	xml.OutOfElem();
}

std::vector<Rect> VJLbp::doTheShit(float scale, int num, int step, float jombogo, float threshold)
{
	std::vector<Rect> windows;//possible windows
	int ysize = 479;
	//threshold *= (integral[479][639] / (480 * 640));
	//int lol=0,olo=0;
	for (int i = num + 1; i > 0; i--) {
		//int scaledThresh = threshold*(ysize*ysize*255); //3ms
		for (int y = 0; y + ysize < 480; y += step) {
			for (int x = 0; x + ysize < 640; x += step) {
				//lol++;
				//if ((skinint[y][x] + skinint[y + ysize][x + ysize] - skinint[y][x + ysize] - skinint[y + ysize][x]) <= scaledThresh)
				//goto skip; 
				//olo++;
				int yscale = ysize / 24;
				int fIdx = 0;
				for (int k = 0; k < 21; k++) {
					float sum = 0;
					for (; fIdx < startIdx[k + 1]; fIdx++) {
						//TODO: Fix IT!!!
						int poins[4][2];
						for (int z = 0; i < 4; z++) {
							poins[z][0] = lines[fIdx][z][0]*yscale+y;
							poins[z][1] = lines[fIdx][z][1]*yscale+x;
						}
						int cval = getSum(poins[1][0], poins[2][0], poins[1][1], poins[2][1]);

						int c = (getSum(poins[0][0], poins[1][0], poins[0][1], poins[1][1]) >= cval ? 128 : 0) |   // 0
								(getSum(poins[1][0], poins[2][0], poins[0][1], poins[1][1]) >= cval ? 64 : 0) |    // 1
								(getSum(poins[2][0], poins[3][0], poins[0][1], poins[1][1]) >= cval ? 32 : 0) |    // 2
								(getSum(poins[2][0], poins[3][0], poins[1][1], poins[2][1]) >= cval ? 16 : 0) |  // 5
								(getSum(poins[2][0], poins[3][0], poins[2][1], poins[3][1]) >= cval ? 8 : 0) |  // 8
								(getSum(poins[1][0], poins[2][0], poins[2][1], poins[3][1]) >= cval ? 4 : 0) |   // 7
								(getSum(poins[0][0], poins[1][0], poins[2][1], poins[3][1]) >= cval ? 2 : 0) |    // 6
								(getSum(poins[0][0], poins[1][0], poins[1][1], poins[2][1]) >= cval ? 1 : 0);
						sum += leaves[fIdx][!(subset[fIdx][c >> 5] & (1 << (c & 31)))];
					}
					if (sum <= stageThresh[k])
						goto skip;
				}
				windows.push_back(Rect(x, y, ysize, ysize));
			skip:;
			}
		}
		ysize /= scale;
	}
	return windows;
}

Mat VJLbp::integrator(Mat target)
{
	Mat gtarget;
	cvtColor(target, gtarget, COLOR_BGR2GRAY);
	uchar *a = gtarget.data;
	uchar *b = target.data;

	int cols = 640, rows = 480;
	for (int i = 0; i < cols; i++) { integral[0][i] = 1; skinint[0][i] = 1; }
	for (int j = 0; j < rows; j++) { integral[j][0] = 1; skinint[j][0] = 1; }
	//Vec3b pixel;
	Mat grey = Mat::zeros(rows, cols, CV_8UC1);
	int lol = 0, olo = 0;
	for (int i = 1; i < rows; i++) {
		lol = target.step*i; olo = 640 * i;
		for (int j = 1; j < cols; j++) {
			uchar lb = b[lol] >> 3, lg = b[lol + 1] >> 3, lr = b[lol + 2] >> 3;
			grey.at<uchar>(i, j) = (charmap[lb][lg][lr]);
			integral[i][j] = a[olo + j] + integral[i - 1][j] + integral[i][j - 1] - integral[i - 1][j - 1];
			skinint[i][j] = charmap[b[lol] >> shit][b[lol + 1] >> shit][b[lol + 2] >> shit] + skinint[i - 1][j] + skinint[i][j - 1] - skinint[i - 1][j - 1];
			lol += 3;
		}
	}
	return grey;
}
