#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\objdetect.hpp>
#include<iostream>
#include<time.h>
#include"VJHaar.h"
#include"VJLbp.h"
using namespace cv;

int main() {
	//read string
	VJHaar Jesus = VJHaar("XML path");
	//VJLbp Jesus = VJLbp("XML path");
	VideoCapture cap = VideoCapture(0);
	Mat frame,gray,integrl;
	float jombogo = 20;
	float bombogo = 0;
	int zet = 0;
	clock_t sum = 0;
	while (true) {
		char c = waitKey(1);
		if (c > 0) { if (c == '+') jombogo++; else if (c == '-') jombogo--; else if (c == '1') bombogo -= 0.02; else if (c == '2') bombogo += 0.02; sum = 0; zet = 0; }
		cap >> frame;
		clock_t hehe = clock();
		frame=Jesus.integrator(frame);
		clock_t haha = clock();
		std::vector<Rect> found = Jesus.doTheShit(1.1, 15, 10, jombogo, bombogo);
		clock_t hoho = clock();
		groupRectangles(found, 1);
		for each (Rect var in found)
		{
			rectangle(frame, var, 255);
		}
		zet++;
		sum += (hoho - haha);
		std::cout <<jombogo<<" "<<bombogo<< " : " << (haha-hehe)<<"-"<<sum/zet<< "\n";
		imshow("Lol", frame);
	}
}