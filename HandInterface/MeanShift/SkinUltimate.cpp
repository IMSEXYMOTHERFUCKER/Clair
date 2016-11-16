#include "SkinUltimate.h"

SkinUltimate::SkinUltimate()
{
}


SkinUltimate::~SkinUltimate()
{
}

SkinUltimate::SkinUltimate(string posPath, string negPath)
{
	//File reading part
	std::ifstream t(posPath);
	std::string strS((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::ifstream z(negPath);
	std::string strN((std::istreambuf_iterator<char>(z)), std::istreambuf_iterator<char>());
	int b = 0, g = 0, r = 0;
	int sums = 0, sumn = 0;

	//fill numberS numberN skinMap from give file
	for (b = 0; b < DIM; b++) {
		for (g = 0; g < DIM; g++) {
			for (r = 0; r < DIM; r++) {
				numberS[b][g][r] = 0;
				numberN[b][g][r] = 0;
				skinMap[b][g][r] = 0;
			}
		}
	}
	b = 0; g = 0; r = 0; int maxS = 0;
	for each (char lol in strS)
	{
		if (lol == ' ') {
			sums += numberS[b][g][r];
			if (numberS[b][g][r] > maxS) maxS = numberS[b][g][r];
			if (r == DIM - 1) {
				r = 0;
				if (g == DIM - 1) {
					g = 0;
					if (b == DIM - 1) {

					}
					else { b++; }
				}
				else { g++; }
			}
			else { r++; }
		}
		else {
			numberS[b][g][r] = numberS[b][g][r] * 10 + (lol - '0');
		}
	}
	b = 0; g = 0; r = 0; int maxN = 0;
	for each (char lol in strN)
	{
		if (lol == ' ') {
			sumn += numberN[b][g][r];
			if (numberN[b][g][r] > maxN) maxN = numberN[b][g][r];
			if (r == DIM - 1) {
				r = 0;
				if (g == DIM - 1) {
					g = 0;
					if (b == DIM - 1) {

					}
					else { b++; }
				}
				else { g++; }
			}
			else { r++; }
		}
		else {
			numberN[b][g][r] = numberN[b][g][r] * 10 + (lol - '0');
		}
	}
	float maxSkin = 0;
	for (b = 0; b < DIM; b++) {
		for (g = 0; g < DIM; g++) {
			for (r = 0; r < DIM; r++) {
				if (numberS[b][g][r] != 0) {
					skinMap[b][g][r] = (Ps*numberS[b][g][r]) / (Ps*numberS[b][g][r] + (1 - Ps)*numberN[b][g][r] * ((double)sums / sumn));	//Ps_c = (Pc_s*Ps) / (Pc_s*Ps + Pc_ns(1-Ps));
					if (skinMap[b][g][r] > maxSkin) maxSkin == skinMap[b][g][r];
				}
			}
		}
	}
	int hehe = 0;
	//calculate charmaps
	for (b = 0; b < DIM; b++) {
		for (g = 0; g < DIM; g++) {
			for (r = 0; r < DIM; r++) {
				//Trees
				if (((r << 3) > 95) && ((g << 3) > 40) && ((b << 3) > 20) && ((r << 3) > (b << 3)) && ((r << 3) > (g << 3) + 15) && ((r << 3) > min((b << 3), (g << 3)) + 15)) {
					map[TREE][b][g][r] = 255;
					map[BAYES_TREE][b][g][r] = skinMap[b][g][r] * 255;
					hehe++;
				}
				else {
					map[TREE][b][g][r] = 0;
					map[BAYES_TREE][b][g][r] = skinMap[b][g][r] * 255 * BT_factor;
				}
				//Prob
				if (((double)numberS[b][g][r] / sums) > ((double)numberN[b][g][r] / sumn))
					map[PROB][b][g][r] = 255;
				else
					map[PROB][b][g][r] = 0;
				//Bayes
				map[BAYES][b][g][r] = skinMap[b][g][r] * 255;
			}
		}
	}
}

cv::Mat SkinUltimate::doIt(cv::Mat target, int type)
{
	if (type != ALL) {
		cv::Mat skin = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Vec3b pixel;
		for (int y = 0; y < target.rows; y++) {
			for (int x = 0; x < target.cols; x++) {
				pixel = target.at<cv::Vec3b>(y, x);
				skin.at<uchar>(y, x) = mapDriver(type, pixel[0], pixel[1], pixel[2]);
			}
		}
		return skin;
	}
	else {
		cv::Mat skinTree = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Mat skinProb = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Mat skinBayes = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Mat skinBayes_Tree = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Mat skinGray_Tree = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Mat skinGray_Prob = cv::Mat::zeros(target.rows, target.cols, CV_8UC1);
		cv::Vec3b pixel;
		for (int y = 0; y < target.rows; y++) {
			for (int x = 0; x < target.cols; x++) {
				pixel = target.at<cv::Vec3b>(y, x);
				skinTree.at<uchar>(y, x) = mapDriver(0, pixel[0], pixel[1], pixel[2]);
				skinProb.at<uchar>(y, x) = mapDriver(1, pixel[0], pixel[1], pixel[2]);
				skinBayes.at<uchar>(y, x) = mapDriver(2, pixel[0], pixel[1], pixel[2]);
				skinBayes_Tree.at<uchar>(y, x) = mapDriver(3, pixel[0], pixel[1], pixel[2]);
				skinGray_Tree.at<uchar>(y, x) = mapDriver(4, pixel[0], pixel[1], pixel[2]);
				skinGray_Prob.at<uchar>(y, x) = mapDriver(5, pixel[0], pixel[1], pixel[2]);
			}
		}
		cv::imshow("Tree", skinTree);
		cv::imshow("Prob", skinProb);
		cv::imshow("Bayes", skinBayes);
		cv::imshow("Bayes_Tree", skinBayes_Tree);
		cv::imshow("Gray_Tree", skinGray_Tree);
		cv::imshow("Gray_Prob", skinGray_Prob);
	}
	return target;
}

uchar SkinUltimate::mapDriver(int type, int b, int g, int r)
{
	if (type < GRAY_TREE) {
		return map[type][b >> 3][g >> 3][r >> 3];
	}
	else {
		return r*(map[type - 4][b >> 3][g >> 3][r >> 3] / 255);
	}
	return 127;
}

