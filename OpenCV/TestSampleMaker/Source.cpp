#include<opencv2\video.hpp>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<iostream>
#include"../SkinUltimate/SkinUltimate.h"
using namespace cv;

int main() {
	SkinUltimate ult = SkinUltimate("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
	VideoCapture cap = VideoCapture(0);
	VideoWriter wri = VideoWriter("../../Datas/Videos/1.mp4", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)));
	Mat frame,gray;
	while (1) {
		cap >> frame;
		waitKey(1);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		Scalar su = sum(gray);
		imshow("gray", frame);
		gray = ult.doIt(frame, BAYES_TREE);
		Scalar suSkin = sum(gray);
		imshow("skin", gray);
		std::cout << su[0]/(frame.cols * frame.rows) <<"||"<< suSkin[0]/ (frame.cols * frame.rows)<<"\n";
	}
}