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
	int h_t = 1;
	int time_;
	Hole* hole;
	int step;
	vector<int> boundary_;
	//ArrayXXf grid;
	vector< vector < vector <Point>> > temp_grid_;
	vector< pair <char, int> > boundary_conditions_;
public:
	Object(int v, double p1, double p2, int step, vector<int> boundary);
	void PrintGrid(int k);
	void CompletionGrid();
	void SetBoundary(vector<int> boundary);
	int BoundaryInit(double x, double y);
	bool InternalSet(double j, double i);
	Point& PointOnNextStep(int x, int y, bool key, bool boundary);
	void NextStep(bool key);
	double SetMuX(int, int);
	double SetMuY(int, int);
	//void SetStep(int step);
	//void CompletionTempGrid();

};