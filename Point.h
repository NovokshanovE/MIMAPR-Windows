#pragma once
class Point {
private:
	double x, y;
	double temp;
	double muX, muY;
	bool boundary;
public:
	Point(double x, double y);
	void SetBoundary(bool key, double mx, double my);
	void SetTemp(double new_temp);
	double GetTemp();
	bool GetBoundary();
};

