#include "Point.h"


Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
void Point::SetBoundary(bool key = false, double mx = 1, double my = 1) {
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
bool Point::GetBoundary() {
	return boundary;
}