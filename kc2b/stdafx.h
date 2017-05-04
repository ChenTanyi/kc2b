// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include "baseFunction.h"
#include "Car.h"
#include "SerialPort.h"

//#define camera

class Car;

const int DISTANCE_ERROR = 40;
const int ANGLE_ERROR = 15;
const int SKIP = 15;
const double PI = acos(-1.0);
const double SIN_THETA_ERROR = sin(ANGLE_ERROR * PI / 180);
const double COS_THETA_ERROR = cos(ANGLE_ERROR * PI / 180);

const cv::Point2f DIRECT[8] = {cv::Point2f(-1,-1),cv::Point2f(-1,0),cv::Point2f(-1,1),cv::Point2f(0,1),
							cv::Point2f(1,1),cv::Point2f(1,0),cv::Point2f(1,-1),cv::Point2f(0,-1)};



void mouse(int , int , int , int , void *);
void on_trackbar(int , void *);


void thin(const cv::Mat & , cv::Mat & , const int iterations = 1);
bool getPosition(cv::Point2f&, cv::Point2f&);
bool enterExitForPuzzle(const cv::Point2f&, const cv::Point2f&, cv::Point2f&, cv::Point2f&);
bool enterExitForBlackLine(const cv::Point2f&, const cv::Point2f&, cv::Point2f&, cv::Point2f&);
void findPath(const cv::Point2f&, const cv::Point2f&);
void simplifyPath(const std::vector<cv::Point2f>&, std::vector<cv::Point2f>&);
void searchPathWithCorner(const cv::Point2f&, const cv::Point2f&, const std::vector<cv::Point2f>&, std::vector<cv::Point2f>&);
void controlCar(const std::vector<cv::Point2f>&);


void push(const std::vector<int>&, const std::vector<cv::Point2f>&, int);
int findNearest(const cv::Point2f&, const std::vector<bool>&);
bool inLine(const cv::Point2f&, const cv::Point2f&, const cv::Point2f&);

double getAngle(const cv::Point2f&, const cv::Point2f&);
bool inMat(const cv::Mat&, const cv::Point2f&); 

// TODO: 在此处引用程序需要的其他头文件
