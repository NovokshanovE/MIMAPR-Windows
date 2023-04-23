#pragma once
#include "Hole.h"
class Square : public Hole {
private:
	double s = 100;
public:
	Square(double p1, double p2) : Hole(p1, p2) {
		Hole::SetVariant(true);
	}
};
