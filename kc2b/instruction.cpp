#include "stdafx.h"

using namespace cv;

void controlCar(const std::vector<Point2f>& path){
	Point2f head, tail;
	extern int model;
	if (!model){
		Car::forward();
		waitKey(500);
	}
	for (auto point : path){
		while (1){
			if (!getPosition(head, tail)){
				Car::stop();
				std::cout << "ERROR!\n";
				exit(1);
			}
			Point2f center = (head + tail) / 2;
			if (norm(center - point) < DISTANCE_ERROR)
				break;
			double angle = getAngle(head - tail, point - center);
			if (- SIN_THETA_ERROR < angle && angle < SIN_THETA_ERROR){
				Car::forward();
			}
			else if (angle < -SIN_THETA_ERROR){
				//Car::stop();
				Car::right();
			}
			else{
				//Car::stop();
				Car::left();
			}
			waitKey(1);
		}
	}
	Car::stop();
	std::cout << "successfully!\n";
}

double getAngle(const Point2f& v1, const Point2f& v2){
	if (!norm(v1) || ! norm(v2)) return 0;
	return (v1.x * v2.y - v1.y * v2.x) / (norm(v1) * norm(v2));
}
