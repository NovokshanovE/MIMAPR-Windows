#pragma once
class Hole {
private:
	double x, y;
	bool variant;
public:
	Hole(double X, double Y);
	double GetX();
	double GetY();
	bool GetVariant();
	void Print();
	void SetVariant(bool variant);
};

