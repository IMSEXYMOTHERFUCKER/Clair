#include<opencv2\core.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\objdetect.hpp>
#include<opencv2\video.hpp>
#include<opencv2\highgui.hpp>
using namespace cv;
#include<stdlib.h>
#include<iostream>
#include<ctime>;
#include"SkinMap.h"

int main() {
	VideoCapture cap = VideoCapture(0);
	CascadeClassifier VJfier = CascadeClassifier("../../Datas/XMLs/BayesHandTr.xml");
	SkinMap skin = SkinMap("../../Datas/Histograms/skinU1.txt", "../../Datas/Histograms/NHistogram.txt");
	Mat frame,grey;
	std::vector<Rect> objects;
	int waitVJ = 0;
	clock_t lol = clock();
	while (true) {	
		waitKey(1);
		cap >> frame;
		
			//cvtColor(frame, grey, COLOR_BGR2GRAY);
		grey = skin.masker(frame);
			clock_t begin = clock();
			VJfier.detectMultiScale(grey, objects);// , 1.1, 3, 0, Size(grey.cols / 4, grey.rows / 4));
				clock_t end = clock();
				for each (Rect tmp1 in objects)
				{
					rectangle(grey, tmp1, 0);
				}
		imshow("image", grey);
		std::cout <<end-begin<< "\n";
	}
}