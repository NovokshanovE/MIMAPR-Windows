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
	int h = 400, w = 500;
	int s = 100;
	int r1 = 50, r2 = 150;
	int h_s, w_s;
	Hole* hole;
	int step = 10;
	//ArrayXXf grid;
	vector< vector < vector <Point>> > temp_grid_;
	vector< pair <char, int> > boundary_conditions_;
public:
	Object(int v, double p1, double p2);
	void PrintGrid(int k);
	void CompletionGrid();
	//void CompletionTempGrid();

};