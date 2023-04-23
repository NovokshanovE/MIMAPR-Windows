#pragma once


#include <iostream>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include <cmath>
#include <vector>
#include "Point.h"



using namespace Eigen;
using namespace std;
///usr/include/eigen3



class Hole {
private:
	double X, Y;
	bool variant;
public:
	Hole(double X, double Y) {
		this->X = X;
		this->Y = Y;
	}
	double getX() {
		return X;
	}
	double getY() {
		return Y;
	}
	bool getVariant() {
		return variant;
	}
	void print() {
		cout << variant << ":" << X << " " << Y << endl;
	}
	void set_var(bool variant) {
		this->variant = variant;
	}
};

class Square : public Hole {
private:
	double S = 100;
public:
	Square(double p1, double p2) : Hole(p1, p2) {
		Hole::set_var(true);
	}
};

class Circle : public Hole {
private:
	double R = 50;
public:
	Circle(double p1, double p2) : Hole(p1, p2) {
		Hole::set_var(false);
	}
};

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