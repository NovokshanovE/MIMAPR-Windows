#pragma once
class Point {
private:
	double x, y;
	double temp;
	double muX, muY;
	bool boundary;
	bool internal;
public:
	Point();
	Point(double x, double y);
	void SetBoundary(bool key, double mx, double my);
	void SetInternal(bool key);
	void SetTemp(double new_temp);
	double GetTemp();
	bool GetBoundary();
	bool GetInternal();
};

