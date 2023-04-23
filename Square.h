#pragma once
#include "Hole.h"
class Square : public Hole {
private:
	double S = 100;
public:
	Square(double p1, double p2) : Hole(p1, p2) {
		Hole::set_var(true);
	}
};
