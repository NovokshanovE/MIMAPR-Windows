#pragma once


#include <iostream>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include "Point.h"
#include "Hole.h"
#include "Circle.h"
#include "Square.h"

using namespace Eigen;
using namespace std;
///usr/include/eigen3









class Object {
private:
	int H = 400, W = 500;
	int S = 100;
	int R1 = 50, R2 = 150;
	int H_s, W_s;
	Hole* hole;
	int step = 10;
	ArrayXXf grid;
	vector< vector < vector <Point>> > tempGrid;
	vector< pair <char, int> > boundary—onditions;
public:
	Object(int v, double p1, double p2);
	void print();
	void completion_grid();
	void completion_temp_grid();

};