#include "stdafx.h"

using namespace cv;

bool getPosition(Point2f& head, Point2f& tail){
	extern VideoCapture cap;
	extern Mat BGRImage, rotated, transmat, frame;
	extern std::vector<int> BGRthreshold, HSVthreshold;
#ifdef camera
	cap >> frame;
#else
	frame = imread("car.jpg");
#endif
	warpPerspective(frame, rotated, transmat, rotated.size());
	rotated.copyTo(BGRImage);
	Mat HSVImage;
	cvtColor(BGRImage, HSVImage, CV_BGR2HSV);
	std::vector<Mat> BGRroad;
	split(BGRImage, BGRroad);
	std::vector<Mat> HSVroad;
	split(HSVImage, HSVroad);


	for (int i = 0 ; i < static_cast<int>(BGRroad.size()) ; ++ i){
		threshold(BGRroad[i], BGRroad[i], BGRthreshold[i], 255, CV_THRESH_BINARY);
		if (i != 0)
			threshold(HSVroad[i], HSVroad[i], HSVthreshold[i], 255, CV_THRESH_BINARY);
		else threshold(HSVroad[i], HSVroad[i], HSVthreshold[i], 255, CV_THRESH_BINARY_INV);
	}

	imshow("Broad", BGRroad[0]);
	imshow("Rroad", BGRroad[2]);
	imshow("Sroad", HSVroad[1]);

	Mat red, blue;
	bitwise_and(BGRroad[2], HSVroad[1], red);
	bitwise_and(BGRroad[0], HSVroad[1], blue);
	Mat kernel = getStructuringElement(0, Size(5,5));
	erode(red, red, kernel);
	erode(blue, blue, kernel);

	Moments momentsRed = moments(red, 1), momentsBlue = moments(blue, 1);

	Point2f carHead(momentsRed.m01 / momentsRed.m00, momentsRed.m10 / momentsRed.m00);
	Point2f carTail(momentsBlue.m01 / momentsBlue.m00, momentsBlue.m10 / momentsBlue.m00);
	imshow("red", red);
	imshow("blue", blue);

	if (!inMat(BGRImage, carHead) || !inMat(BGRImage, carTail)){
		std::cout << "Can't find the car.\n";
		return 0;
	}
	else {
		circle(rotated, Point(carHead.y, carHead.x), 3, Scalar(0,255,0), 2);
		circle(rotated, Point(carTail.y, carTail.x), 3, Scalar(0,255,0), 2);
		imshow("colorImg", rotated);
		head = carHead;
		tail = carTail;
	}
	waitKey(1);
	return 1;
}