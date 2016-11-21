#include "VJHaar.h"
#include <opencv2\imgproc.hpp>
#include<opencv2\highgui.hpp>
#include<fstream>
#include<iostream>
#include<omp.h>
float VJHaar::get(CMarkup &xml,int num)
{
	std::wstring lol=xml.GetData();
	std::vector<std::wstring> v;
	split(lol, v, ' ');
	return std::stof(v[num]);
}

VJHaar::VJHaar()
{
}

VJHaar::~VJHaar()
{
}

VJHaar::VJHaar(String XMLname)
{
	int numbers[HDIM][HDIM][HDIM];
	int numbern[HDIM][HDIM][HDIM];
	float skinmap[HDIM][HDIM][HDIM];

	std::ifstream t("../../Datas/Histograms/skinU1.txt");
	std::string strS((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::ifstream z("../../Datas/Histograms/NHistogram.txt");
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
			if (r == HDIM-1) {
				r = 0;
				if (g == HDIM-1) {
					g = 0;
					if (b == HDIM-1) {

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
	float boom3 = sums/sumn;
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

	CMarkup xml;
	xml.Load(MCD_T("../../Datas/XMLs/HandBack4.xml"));
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
										bool joom=xml.FindElem(MCD_T("internalNodes"));
										recIds[weakI] = get(xml, 3 + 1);//recId
										thresholds[weakI] = get(xml,4+1);
										xml.FindElem(MCD_T("leafValues"));
										leaves[weakI][0] = get(xml,1+1);
										leaves[weakI][1] = get(xml,2+1);
									xml.OutOfElem();
								}
								xml.OutOfElem();
						xml.OutOfElem();
					}
					
				xml.OutOfElem();
				xml.FindElem(MCD_T("features"));
				xml.IntoElem();
					for (int i = 0; i<NumOfFeatures+1; i++) {
						xml.FindElem(MCD_T("_"));
						xml.IntoElem();
							xml.FindElem(MCD_T("rects"));
							xml.IntoElem();
								for (int j = 0; j<2; j++) {
									xml.FindElem(MCD_T("_"));
									reccompressed[i][j] = (((int)get(xml, 5 + 1) + 1) << 24) | (((int)get(xml, 4 + 1)+ (int)get(xml, 2 + 1)) << 18) | (((int)get(xml, 3 + 1)+ (int)get(xml, 1 + 1)) << 12) | (((int)get(xml, 2 + 1)) << 6) | ((int)get(xml, 1 + 1));
								}
								if (xml.FindElem(MCD_T("_"))) {
									recnum[i] = 2;
									reccompressed[i][2] = (((int)get(xml, 5 + 1) + 1) << 24) | (((int)get(xml, 4 + 1)+ (int)get(xml, 2 + 1)) << 18) | (((int)get(xml, 3 + 1)+ (int)get(xml, 1 + 1)) << 12) | (((int)get(xml, 2 + 1)) << 6) | ((int)get(xml, 1 + 1));
								}
								else { recnum[i] = 1;}
							xml.OutOfElem();
						xml.OutOfElem();
					}
				xml.OutOfElem();
		xml.OutOfElem();
	xml.OutOfElem();
	cols = 640;
	rows = 480;
	integral = new int*[rows];
	for (int i = 0; i < rows; ++i)
		integral[i] = new int[cols];
	skinint = new int*[rows];
	for (int i = 0; i < rows; ++i)
		skinint[i] = new int[cols];
}

Mat VJHaar::integrator(Mat target)
{
	Mat gtarget;
	cvtColor(target, gtarget, COLOR_BGR2GRAY);
	uchar *a = gtarget.data;
	uchar *b = target.data;

	for (int i = 0; i < cols; i++) { integral[0][i] = 1; skinint[0][i] = 1; }
	for (int j = 0; j < rows; j++) { integral[j][0] = 1; skinint[j][0] = 1; }
	//Vec3b pixel;
	Mat grey = Mat::zeros(rows, cols, CV_8UC1);
	int lol = 0, olo = 0, total = 0;
	for (int i = 1; i < rows; i++) {
		lol = target.step*i; olo = cols * i;
		for (int j = 1; j < cols; j++) {
			uchar lb = b[lol]>>3, lg = b[lol + 1]>>3, lr = b[lol + 2]>>3;
			grey.at<uchar>(i, j) = (charmap[lb][lg][lr]);
			integral[i][j] = a[olo + j] + integral[i - 1][j] + integral[i][j - 1] - integral[i - 1][j - 1];
			skinint[i][j] = charmap[b[lol] >> 3][b[lol + 1] >> 3][b[lol + 2] >> 3] + skinint[i - 1][j] + skinint[i][j - 1] - skinint[i - 1][j - 1];
			lol += 3;
			total++;
		}
	}
	Scalar st = sum(gtarget);
	Scalar ss = sum(grey);
	//std::cout << " Light: " << st[0] / total << " Skin: " << ss[0] / total<<" ";
	return target;
}

std::vector<Rect> VJHaar::doTheShit(float scale, int num, int step, float jombogo, float threshold, float &percentage)
{	
	std::vector<Rect> windows;//possible windows
	int ysize = rows-1;
	int xsize = ysize * Xsize / Ysize;
	threshold *= (255);
	int lol=0,olo=0;
	for (int i = num+1; i > 0; i--) { //each scale
		int scaledThresh = threshold*(ysize*xsize);
		for (int y = 0; y+ysize < rows; y += step) { //each window
			for (int x = 0; x+xsize < cols; x += step) {
				lol++;
				if ((skinint[y][x] + skinint[y + ysize][x + xsize] - skinint[y][x + xsize] - skinint[y + ysize][x]) <= scaledThresh)
					goto skip; 
				olo++;
				int scale = ysize / Ysize; //setting up for VJ
				int varianceNormFactor = ysize*xsize*jombogo;
				int fIdx = 0;
				for (int k = 0; k < ActiveStages; k++) {
					float sum = 0;
					for (; fIdx < startIdx[k + 1]; fIdx++) {
						int sumf = 0;
						int tmp = recIds[fIdx];
						int* rec = reccompressed[tmp];
						for (int subrec = recnum[tmp]; subrec>=0; subrec--){
							int recomp = rec[subrec];
							int x0 = x + scale*(recomp&(63)); recomp = recomp >> 6;
							int *y0 = integral[y + scale*(recomp&(63))]; recomp = recomp >> 6;
							int x1 = x + scale*(recomp&(63)); recomp = recomp >> 6;
							int *y1 = integral[y + scale*(recomp&(63))]; recomp = recomp >> 6;
							sumf += (y0[x0] + y1[x1] - y0[x1] - y1[x0])*(recomp-1);
						}
						sum+=leaves[fIdx][!(sumf < varianceNormFactor*thresholds[fIdx])];
					}
					if (sum <= stageThresh[k])
						goto skip;
				}
				windows.push_back(Rect(x,y,xsize,ysize));
			skip:;
			}
		}
		ysize /= scale;
		xsize /= scale;
	}
	percentage=(float)(olo*100 / lol);
	return windows;
}
