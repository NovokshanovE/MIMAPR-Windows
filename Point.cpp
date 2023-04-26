#include "Point.h"

Point::Point() {
	double x = 0, y = 0;
	double temp = 0;
	double muX = 0, muY = 0;
	bool boundary = false;
	bool internal = false;
}
Point::Point(double X, double Y) {
	x = X;
	y = Y;
	temp = 0;
	boundary = 0;
	
}
void Point::SetInternal(bool key) {
	this->internal = key;
}
void Point::SetBoundary(int key, double mx, double my) {
	boundary = key;
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