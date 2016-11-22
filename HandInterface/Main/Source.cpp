#include<opencv2\highgui.hpp>
#include<opencv2\core.hpp>
#include<opencv2\imgproc.hpp>
using namespace cv;
#include<iostream>
#include<stdlib.h>

int main() {
	VideoCapture cap = VideoCapture(0);
	Mat frame;
	//init
	bool found = false, started=false, tracking=false;
	int waited = 0, waitLim = 100;
	std::vector<Rect> windows;
	while (1) {
		char c = waitKey(1);
		if (c < 0) {
			if (c == '1') {
				started = true; //start whole process
			}
			else {
				started = false; //stop whole process
			}
		}
		cap >> frame;
		if (!started) { waited = 0; tracking = false; found = false; }
		if (waited < waitLim) {
			//detect hand windows and send to "windows"
			if (!windows.empty()) { //we found hands
				//create histogram and initialize tracking
				tracking = true;
				found = true;
			}
		}
		else {
			if (tracking) {
				//Meanshift
				if () { //check if we still tracking hand
					//abort tracking and start detection
					found = false;
				}
				else {
					waited++;
				}
			}
		}
		if (found) {
			//hand segmentation in "frame(windows[0])"
			//gesture recognition
		}
	}
}