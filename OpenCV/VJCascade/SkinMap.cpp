#include "SkinMap.h"

SkinMap::SkinMap()
{
}

SkinMap::~SkinMap()
{
}

SkinMap::SkinMap(String skinHist, String nonsHist)
{
	std::ifstream t(skinHist);
	std::string strS((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::ifstream z(nonsHist);
	std::string strN((std::istreambuf_iterator<char>(z)), std::istreambuf_iterator<char>());
	int b = 0, g = 0, r = 0;
	int sums = 0, sumn = 0;

	//get numbers
	for (b = 0; b < 32; b++) {
		for (g = 0; g < 32; g++) {
			for (r = 0; r < 32; r++) {
				numbers[b][g][r] = 0;
				numbern[b][g][r] = 0;
				skinmap[b][g][r] = 0;
			}
		}
	}
	b = 0; g = 0; r = 0;
	for each (char lol in strS)
	{
		if (lol == ' ') {
			sums += numbers[b][g][r];
			if (r == 31) {
				r = 0;
				if (g == 31) {
					g = 0;
					if (b == 31) {
						
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
			if (r == 31) {
				r = 0;
				if (g == 31) {
					g = 0;
					if (b == 31) {

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
	float boom3 = 0.002019;
	for (b = 0; b < 32; b++) {
		for (g = 0; g < 32; g++) {
			for (r = 0; r < 32; r++) {
				if (numbers[b][g][r] != 0) {
					int boom1 = numbers[b][g][r], boom2 = numbern[b][g][r];
					skinmap[b][g][r] = (Ps*boom1) / (Ps*boom1 + (1 - Ps) * boom2 * boom3);	//Ps_c = (Pc_s*Ps) / (Pc_s*Ps + Pc_ns(1-Ps));
					if (skinmap[b][g][r] > maxskin) maxskin == skinmap[b][g][r];
				}
			}
		}
	}

	for (b = 0; b < 32; b++) {
		for (g = 0; g < 32; g++) {
			for (r = 0; r < 32; r++) {
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
				charmap[b][g][r] = skinmap[b][g][r] * 255 * 0.5;
				}
				//if (((double)numbers[b][g][r] / sums) > ((double)numbern[b][g][r] / sumn))
				//	charmap[b][g][r] = 255;
				//else
				//	charmap[b][g][r] = 0;
			}
		}
	}
}

Mat SkinMap::masker(Mat target)
{
	int cols = target.cols;
	int rows = target.rows;
	Vec3b pixel;
	Mat grey = Mat::zeros(rows, cols, CV_8UC1);
	for (int i = 1; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			pixel = target.at<Vec3b>(i, j);
			int b = pixel[0] >> 3, g = pixel[1] >> 3, r = pixel[2] >> 3;
			int tmp = 0;
			grey.at<uchar>(i, j) = charmap[b][g][r]; 
		}
	}
	return grey;
}

std::vector<cv::Rect> SkinMap::finder(double threshold, double scale, int num, int **integral, int step)
{
	std::vector<Rect> found = std::vector<Rect>();
	int rows = 480, cols = 640;
	int ysize = 480;
	threshold *= integral[479][639]/(480*640);
	for (int i = 0; i < num; i++) {
		//int ysize = rows, xsize = cols, scaledThresh = threshold;
		for (int y = ysize; y < rows; y = y + step) {
			for (int x = ysize; x < cols; x = x + step) {
				int scaledThresh = threshold*(ysize*ysize);
				if ((integral[y][x] + integral[y - ysize][x - ysize] - integral[y][x - ysize] - integral[y - ysize][x]) > scaledThresh) {
					for each (Rect tmp in found)
					{
						if ((tmp.x <= (x - ysize)) && (tmp.y <= (y - ysize)) && (tmp.width+tmp.x >= x) && (tmp.height+tmp.y >= y)) {
							goto skip;
						}
					}
					found.push_back(Rect(x - ysize, y - ysize, ysize, ysize));
				skip:;
				}
			}
		}
		ysize = ysize / scale;;
	}

	return found;
}

