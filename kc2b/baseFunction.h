#pragma once

#include <opencv2\opencv.hpp>

template <typename T>
T sqr(T a){return a * a;}

struct Point2fCmp{
	bool operator() (const cv::Point2f& a, const cv::Point2f& b) const {
		return a.x < b.x || (a.x == b.x && a.y < b.y);
	}
};
