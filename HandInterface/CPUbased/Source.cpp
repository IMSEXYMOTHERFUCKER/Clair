#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\objdetect.hpp>
#include<iostream>
#include<time.h>
#include<math.h>
#include"VJHaar.h"
#include"VJLbp.h"
using namespace cv;

int main() {
	//read string
	VJHaar Jesus = VJHaar("XML path");
	//VJLbp Jesus = VJLbp("XML path");
	String names[14] = { "0-0","32-0","32-32","64-0","64-32","64-64","96-0","96-32","96-64","128-0","128-32","128-64","128-96","160-96" };
	for (int q = 0; q < 14; q++) {
		VideoCapture cap = VideoCapture("../../Datas/TestHand/" + names[q] + ".mp4");
		Mat frame, gray, integrl;
		float jombogo = 20;
		float bombogo = 0;
		float optimal = 0, frameNum = 0, avrgPercent = 0;
		while (true) {
			char c = waitKey(1);
			if (c > 0) { if (c == '+') jombogo++; else if (c == '-') jombogo--; else if (c == '1') bombogo -= 0.02; else if (c == '2') bombogo += 0.02; }
			cap >> frame;
			if (frame.empty()) break;
			frame = Jesus.integrator(frame);
			bombogo = 0;
			float percentage;
			std::vector<Rect> found = Jesus.doTheShit(1.1, 15, 10, jombogo, bombogo, percentage);
			groupRectangles(found, 1);
			bool handExist = false;
			for each (Rect var in found) { handExist = true; rectangle(frame, var, Scalar(0, 255, 0), 4); }
			for (; found.size() > 0; bombogo += 0.01) {
				found.clear();
				found = Jesus.doTheShit(1.1, 15, 10, jombogo, bombogo, percentage);
				groupRectangles(found, 1);
			}
			bombogo -= 0.02;
			found.clear();
			found = Jesus.doTheShit(1.1, 15, 10, jombogo, bombogo, percentage);
			groupRectangles(found, 1);
			for each (Rect var in found) { rectangle(frame, var, 255); }
			//std::cout <<"\n|"<<percentage<< "|\n";
			imshow("Lol", frame);

			if (bombogo > 0) {
				optimal += bombogo;
				avrgPercent += percentage;
				frameNum++;
			}
		}
		std::cout << "\nAverage Threshold: " << optimal / frameNum << " || Percentage: " << avrgPercent / frameNum;
	}
	while (true) {

	}
}