#include "Hole.h"
#include <iostream>
using namespace std;

Hole::Hole(double X, double Y) {
	this->X = X;
	this->Y = Y;
}
double Hole::getX() {
	return X;
}
double Hole::getY() {
	return Y;
}
bool Hole::getVariant() {
	return variant;
}
void Hole::print() {
	cout << variant << ":" << X << " " << Y << endl;
}
void Hole::set_var(bool variant) {
	this->variant = variant;
}