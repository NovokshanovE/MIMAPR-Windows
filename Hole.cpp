#include "Hole.h"
#include <iostream>
using namespace std;

Hole::Hole(double x, double y) {
	this->x = x;
	this->y = y;
}
double Hole::GetX() {
	return x;
}
double Hole::GetY() {
	return y;
}
bool Hole::GetVariant() {
	return variant;
}
void Hole::Print() {
	cout << variant << ":" << x << " " << y << endl;
}
void Hole::SetVariant(bool variant) {
	this->variant = variant;
}