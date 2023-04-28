#pragma once
#include "Hole.h"

class Circle : public Hole {
private:
	double r = 50;
public:
	Circle(double p1, double p2) : Hole(p1, p2) {
		Hole::SetVariant(false);
	}
};
