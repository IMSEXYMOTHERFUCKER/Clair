#include "SkinMaskClassifier2.h"

SkinMaskClassifier2::SkinMaskClassifier2()
{
}

SkinMaskClassifier2::~SkinMaskClassifier2()
{
}

Mat SkinMaskClassifier2::masker(Mat target, int **integral)
{
	int cols = 640;
	int rows = 480;
	for (int i = 0; i < cols; i++) integral[0][i] = 0;
	for (int j = 0; j < rows; j++) integral[j][0] = 1;
	Vec3b pixel;
	Mat grey = Mat::zeros(rows, cols, CV_8UC1);
	for (int i = 1; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			pixel = target.at<Vec3b>(i, j); int tmp = 0;
			if (pixel[2] > 95) {
				if (pixel[1] > 40) {
					if (pixel[0] > 20) {
						if (pixel[2] > pixel[1]) {
							if (pixel[2] > pixel[0]) {
								if (pixel[2] > pixel[1] + 15) {
									if (pixel[2] > min(pixel[0],pixel[1]) + 15) {
										tmp = 1; grey.at<uchar>(i, j) = 255;
									}
								}
							}
						}
					}
				}
			}
			//(R-95)&(R-G-15)&(R-B)
			tmp = tmp + integral[i - 1][j];
			tmp = tmp + integral[i][j - 1];
			tmp = tmp - integral[i - 1][j - 1];
			integral[i][j] = tmp;
		}
	}
	return grey;
}


std::vector<cv::Rect> SkinMaskClassifier2::finder(double threshold, double scale, int num, int **integral, int step)
{
	std::vector<Rect> found = std::vector<Rect>();
	found.push_back(Rect(638, 478, 1, 1));
	int ysize = 480, xsize = 640;
	for (int i = 0; i < num; i++) {
		ysize = ysize/scale; xsize=xsize/scale; int scaledThresh = threshold*ysize*ysize;
		for (int y = ysize; y < 480; y = y + step) {
			for (int x = ysize; x < 640; x = x + step) {
				int et = (integral[y][x] + integral[y - ysize][x - ysize] - integral[y][x - ysize] - integral[y - ysize][x]);
				if ( et > scaledThresh) {
					//for each (Rect zet in found)
					//{
						//if ((zet.x <= (x - xsize)) && (zet.y <= (y - ysize))&& (zet.width+zet.x >= x) && (zet.height+zet.y >= y)) {
							//goto skip;
						//};
					//}
					found.push_back(Rect(x - ysize, y - ysize, ysize, ysize));
				skip:;
				}
			}
		}
	}
	found.erase(found.begin());
	return found;
}

