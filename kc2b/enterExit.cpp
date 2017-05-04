#include "stdafx.h"

using namespace cv;

extern Mat thinroad;
extern std::set<Point2f, Point2fCmp> pointSet;

bool enterExitForPuzzle(const Point2f& head, const Point2f& tail, Point2f& enter, Point2f& exit){
	enter = Point2f(-1,-1);
	bool finish = 0;
	for (int i = 0; i < thinroad.rows; ++ i){
		if (finish) break;
		int flag = 0;
		Point2f now(i,0);
		for (auto direct : DIRECT){
			if (inMat(thinroad, now + direct) && pointSet.find(now + direct) != pointSet.end())
				++ flag;
		}
		if (flag == 3){
			if (enter == Point2f(-1,-1))
				enter = now;
			else {
				exit = now;
				finish = 1;
			}
		}
		now = Point2f(i, thinroad.cols - 1);
		for (auto direct : DIRECT){
			if (inMat(thinroad, now + direct) && pointSet.find(now + direct) != pointSet.end())
				++ flag;
		}
		if (flag == 3){
			if (enter == Point2f(-1,-1))
				enter = now;
			else {
				exit = now;
				finish = 1;
			}
		}
	}
	for (int j = 0; j < thinroad.cols; ++ j){
		if (finish) break;
		int flag = 0;
		Point2f now(0,j);
		for (auto direct : DIRECT){
			if (inMat(thinroad, now + direct) && pointSet.find(now + direct) != pointSet.end())
				++ flag;
		}
		if (flag == 3){
			if (enter == Point2f(-1,-1))
				enter = now;
			else {
				exit = now;
				finish = 1;
			}
		}
		now = Point2f(thinroad.rows - 1, j);
		for (auto direct : DIRECT){
			if (inMat(thinroad, now + direct) && pointSet.find(now + direct) != pointSet.end())
				++ flag;
		}
		if (flag == 3){
			if (enter == Point2f(-1,-1))
				enter = now;
			else {
				exit = now;
				finish = 1;
			}
		}
	}
	if (!finish) return 0;
	Point2f center = (head + tail) / 2;
	if (norm(center - enter) > norm(center - exit)){
		std::swap(enter, exit);
	}
	return 1;
}

bool enterExitForBlackLine(const Point2f& head, const Point2f& tail, Point2f& enter, Point2f& exit){
	enter = Point2f(-1, -1);
	bool finish = 0;
	for (auto point : pointSet){
		if (finish) break;
		int flag = 0;
		for (auto direct : DIRECT){
			if (inMat(thinroad, point + direct) && pointSet.find(point + direct) != pointSet.end())
				++ flag;
		}
		if (flag == 1){
			if (enter == Point2f(-1, -1))
				enter = point;
			else {
				exit = point;
				finish = 1;
			}
		}
	}
	if (!finish) return 0;
	Point2f center = (head + tail) / 2;
	if (norm(center - enter) > norm(center - exit))
		std::swap(enter, exit);
	return 1;
}


bool inMat(const Mat& mat, const Point2f& point){
	return 0 <= point.x && point.x < mat.rows && 0 <= point.y && point.y < mat.cols;
}