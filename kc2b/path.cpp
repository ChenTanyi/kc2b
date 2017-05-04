#include "stdafx.h"

using namespace cv;

extern std::vector<Point2f> corners, path;
extern std::vector<Vec4i> lines;
//extern std::map<Point2f, bool, Point2fCmp> cornersMap;
extern std::set<Point2f, Point2fCmp> pointSet;
extern Mat thinroad, rotated;

void findPath(const Point2f& start, const Point2f& end){
	std::vector<Point2f> queue;
	std::map<Point2f, bool, Point2fCmp> visit;
	std::vector<int> f;
	queue.push_back(start);
	visit[start] = 1;
	f.push_back(-1);
	for (int head = 0 ; head < static_cast<int>(queue.size()); ++ head){
		Point2f now = queue[head];
		for (auto direct : DIRECT){
			if (pointSet.find(now + direct) != pointSet.end() && !visit.count(now + direct)){
				queue.push_back(now + direct);
				visit[now + direct] = 1;
				f.push_back(head);
			}
			if (now + direct == end){
				push(f, queue, static_cast<int>(queue.size() - 1));
				return;
			}
		}
	}
}

void push(const std::vector<int>& f, const std::vector<Point2f>& queue, int now){
	if (~now){
		push(f, queue, f[now]);
		path.push_back(queue[now]);
	}
}

void simplifyPath(const std::vector<Point2f>& path, std::vector<Point2f>& simplePath){
	simplePath.push_back(path[0]);
	for (int i = SKIP; i + SKIP < static_cast<int>(path.size()); ++ i){
		Point2f v1 = path[i] - path[i - SKIP], v2 = path[i + SKIP] - path[i];
		double cosTheta = (v1.x * v2.x + v1.y * v2.y) / (norm(v1) * norm(v2));
		if (cosTheta < COS_THETA_ERROR){
			simplePath.push_back(path[i]);
			i += SKIP;
		}
	}
	simplePath.push_back(*path.rbegin());
}

void searchPathWithCorner(const cv::Point2f& start, const cv::Point2f& end, const std::vector<cv::Point2f>& corners, std::vector<cv::Point2f>& route){
	std::vector<bool> visit(corners.size(), 0);
	int flag = 0;
	auto now = *corners.begin();
	for (int i = 0 ; i < static_cast<int>(corners.size()) ; ++ i){
		if (norm(now - corners[i]) > norm(start - corners[i])){
			now = corners[i];
			flag = i;
		}
	}
	visit[flag] = 1;
	route.push_back(now);
	while (1){
		auto next = findNearest(now, visit);
		if (~next){
			visit[next] = 1;
			now = corners[next];
			route.push_back(now);
		}
		else break;
	}
}

int findNearest(const Point2f& now, const std::vector<bool>& flag){
	double min = DBL_MAX;
	int select = -1;
	for (auto line : lines){
		Point2f s(line[0], line[1]), e(line[2], line[3]);
		if (!inLine(s, e, now))
			continue;
		for (int i = 0 ; i < static_cast<int>(corners.size()) ; ++ i){
			if (!inLine(s, e, corners[i]))
				continue;
			double dis = norm(corners[i] - now);
			if (dis < min && !flag[i]){
				min = dis;
				select = i;
			}
		}
	}
	if (~select) return select;
	for (int i = 0 ; i < static_cast<int>(corners.size()) ; ++ i){
		double dis = norm(corners[i] - now);
		if (dis < min && !flag[i]){
			min = dis;
			select = i;
		}
	}
	return select;
}

bool inLine(const Point2f& a, const Point2f& b, const Point2f& c){
	Point2f v1 = c - a, v2 = c - b;
	return abs(v1.x * v2.y - v1.y * v2.x) < norm(v1) * norm(v2) * SIN_THETA_ERROR;
		//&& norm(v1) - norm(a - b) < DISTANCE_ERROR  && norm(v2) - norm(a - b) < DISTANCE_ERROR;
}