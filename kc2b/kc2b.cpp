// kc2b.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace cv;

int step = 0, Threshold = 100, model = 0;
Mat frame, BWroad, GRroad, thinroad, transmat, rotated, BGRImage;
CSerialPort com1;
VideoCapture cap;
Point2f start = Point2f(-1, -1), end = Point2f(-1, -1);
std::vector<Point2f> originpoints(4), newpoints(4), corners, path;
std::vector<bool> visit;
std::vector<Vec4i> lines;
std::map<Point2f, bool, Point2fCmp> cornersMap;
std::set<Point2f, Point2fCmp> pointSet;
std::vector<int> BGRthreshold(3,85), HSVthreshold(3,100);
Car car;

int _tmain(int argc, _TCHAR* argv[])
{

#ifdef camera
	if (!com1.InitPort(6)){
		std::cout << "There is sth wrong with you bluetooth.\n";
		return -1;
	}
	Car::speedUp(4);
	Car::speedUp(5);
	Car::stop();
	/*namedWindow("win",CV_WINDOW_NORMAL);
	Car::forward();
	waitKey();
	Car::left();
	waitKey();
	Car::right();
	waitKey();
	Car::forward();
	waitKey();
	return 0;*/
#endif
	Mat choice = imread("choice.jpg");
	if (choice.data){
		imshow("choice", choice);
		createTrackbar("Model", "choice", &model, 1);
		waitKey();
	}

	if (model) std::cout << "You choose puzzle.\n";
	else std::cout << "You choose black line.\n";
	destroyWindow("choice");
#ifdef camera
	cap.open(0);
	if (!cap.isOpened()){
		std::cout << "There is sth wrong with your camera.\n";
		return -1;
	}
	while (1){
		cap >> frame;
		imshow("colorImg", frame);
		if (waitKey(1) > 0) break;
	}
	cap >> frame;
#else
	frame = imread("saveImage.jpg");
	if (!frame.data){
		return -1;
	}
#endif

	newpoints[0] = Point2f(0,0);
	newpoints[1] = Point2f(699,0);
	newpoints[2] = Point2f(0,499);
	newpoints[3] = Point2f(699,499);

	while (1){
		imshow("colorImg", frame);
		createTrackbar("Step","colorImg", &step, 5, 0);
		setMouseCallback("colorImg", mouse);
		if (waitKey(1) > 0 && step >= 4) break;
	}

	transmat = getPerspectiveTransform(originpoints, newpoints);
	rotated = Mat(500, 700, frame.type());
	warpPerspective(frame, rotated, transmat, rotated.size());
	rotated.copyTo(BGRImage);
	destroyWindow("colorImg");
	imshow("colorImg", rotated);
	if (!model) Threshold = 75;
	createTrackbar("Step","colorImg", &step, 5, 0);
	createTrackbar("Threshold", "colorImg", &Threshold, 254, on_trackbar);

	if (model){
		cvtColor(rotated, GRroad, CV_BGR2GRAY);	
	}
	else {
		Mat tmpGRroad;
		cvtColor(rotated, tmpGRroad, CV_BGR2GRAY);
		Mat kernel = getStructuringElement(0, Size(500,700));
		morphologyEx(tmpGRroad, GRroad, MORPH_BLACKHAT, kernel);
	}
	on_trackbar(Threshold, 0);
	waitKey();

	goodFeaturesToTrack(thinroad, corners, 100, 0.01, 10);
	for (auto corner : corners){
		cornersMap[corner] = 1;
		circle(rotated, corner, 3, Scalar(255,0,0), 2);
	}

	HoughLinesP(thinroad, lines, 1, CV_PI / 180, 5);
	for (auto it : lines){
		line(rotated, Point(it[0], it[1]), Point(it[2], it[3]), Scalar(0,255,0));
	}

	imshow("colorImg", rotated);
	
	std::cout << "Place the car in the mat, then make step to 5.\n";
	while (step < 5){
		waitKey(1);
	}

	for (int i = 0 ; i < thinroad.rows; ++ i){
		for (int j = 0 ; j < thinroad.cols; ++ j){
			if (thinroad.at<uchar>(i, j) == 255){
				pointSet.insert(Point2f(i,j));
			}
		}
	}

#ifdef camera
	cap >> frame;
#else
	frame = imread("car.jpg");
#endif
	warpPerspective(frame, rotated, transmat, rotated.size());
	rotated.copyTo(BGRImage);
	imshow("colorImg", rotated);
	waitKey();

	BGRthreshold[0] = 65;
	BGRthreshold[2] = 65;
#ifdef camera
	while (1){
		cap >> frame;
		warpPerspective(frame, rotated, transmat, rotated.size());
		imshow("colorImg",rotated);
		if (waitKey(1) > 0) break;
	}
	destroyWindow("colorImg");
#endif

	while (true){
		Point2f a ,b;
		namedWindow("change for thershold");
		createTrackbar("Threshold B", "change for thershold", &BGRthreshold[0], 255);
		createTrackbar("Threshold R", "change for thershold", &BGRthreshold[2], 255);
		createTrackbar("Threshold S", "change for thershold", &HSVthreshold[1], 255);
		getPosition(a, b);
		if (waitKey(10) > 0) break;
	}
	destroyWindow("change for thershold");

	Point2f carHead, carTail;
	getPosition(carHead, carTail);
	int flag = 0;
	/*if (model) {
		if (enterExitForPuzzle(carHead, carTail, start, end))
			flag = 1;
	}
	else if (enterExitForBlackLine(carHead, carTail, start, end))
		flag = 1;*/
	if (!flag){
		std::cout << "The find of enter and exit has something wrong, please click for the enter and exit.\n";
		setMouseCallback("colorImg", mouse);
		while (end == Point2f(-1, -1)){
			waitKey(1);
		}
		Point2f tmpStart = *pointSet.begin(), tmpEnd = *pointSet.begin();
		for (auto point : pointSet){
			if (norm(start - point) < norm(start - tmpStart))
				tmpStart = point;
			if (norm(end - point) < norm(end - tmpEnd))
				tmpEnd = point;
		}
		start = tmpStart, end = tmpEnd;
	}
	destroyWindow("colorImg");
	circle(rotated, Point(start.y, start.x), 10, Scalar(255, 255, 0), 3);
	circle(rotated, Point(end.y, end.x), 10, Scalar(255, 255, 0), 3);
	imshow("colorImg", rotated);
	waitKey();
	findPath(start, end);
	std::vector<Point2f> route;
	if (path.size()){
		simplifyPath(path, route);
	}
	else {
		if (model){
			std::cout << "There is sth wrong with finding path!\n";
			return -1;
		}
		else {
			searchPathWithCorner(start, end, corners, route);
			for (auto& point : route){
				point = Point2f(point.y, point.x);
			}
		}
	}
	for (auto point : route) {
		std::cout << point << "\n";
		circle(rotated, Point(point.y, point.x), 3, Scalar(0,0,255), 2);
	}
	imshow("colorImg", rotated);
	waitKey();

#ifdef camera
	controlCar(route);
#endif

	waitKey();

	return 0;
}
