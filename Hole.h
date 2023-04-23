#pragma once
class Hole {
private:
	double X, Y;
	bool variant;
public:
	Hole(double X, double Y);
	double getX();
	double getY();
	bool getVariant();
	void print();
	void set_var(bool variant);
};

