#include "Point.h"

Point::Point() {
	double x = 0, y = 0;
	double temp = 0;
	double muX = 0, muY = 0;
	bool boundary = false;
	bool internal = false;
	time = 0;
}
Point::Point(double X, double Y) {
	x = X;
	y = Y;
	temp = 0;
	boundary = 0;
	time = 0;
	
}
Point& Point::operator=(Point& p) {
	x = p.x;
	y = p.y;
	temp = p.temp;
	muX = p.muX;
	muY = p.muY;
	boundary = p.boundary;
	internal = p.internal;
	time = p.time + 1;
	return *this;
}
void Point::SetInternal(bool key) {
	this->internal = key;
}
void Point::SetBoundary(int key) {
	boundary = key;
	
}
void Point::SetMu(double mx, double my) {
	muX = mx;
	muY = my;
}
void Point::SetTemp(double new_temp) {
	temp = new_temp;
}
double Point::GetTemp() {
	return temp;
}
int Point::GetBoundary() {
	return boundary;
}

bool Point::GetInternal() {
	return internal;
}