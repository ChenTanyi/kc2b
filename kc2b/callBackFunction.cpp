#include "stdafx.h"

using namespace cv;

extern int step, Threshold;
extern Mat GRroad, BWroad, thinroad;
extern std::vector<Point2f> originpoints;
extern Point2f start, end;

void mouse(int mouseevent, int x, int y, int, void*){
	if (mouseevent == EVENT_LBUTTONDOWN){
		std :: cout << x << " " << y << "\n";
		if (step < 4)
			originpoints[step++] = Point2f(x, y);
		else if (step == 5){
			static int num = 0;
			if ((num ++) == 0){
				start = Point2f(y, x);
			}
			else end = Point2f(y, x);
		}
	}
}

void on_trackbar(int, void*){
	threshold(GRroad, BWroad, Threshold, 255, CV_THRESH_BINARY);
	Mat tmpBWroad;
	Mat kernel = getStructuringElement(0, Size(5,5));
	morphologyEx(BWroad, tmpBWroad, MORPH_CLOSE, kernel);
	morphologyEx(tmpBWroad, BWroad, MORPH_OPEN, kernel);
    thin(BWroad, thinroad, 200);
    imshow("thinroad",thinroad);
}
