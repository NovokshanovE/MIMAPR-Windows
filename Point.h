#pragma once
class Point {
private:
	double x, y;
	double temp;
	double muX, muY;
	int boundary;
	bool internal;
	int time;
public:
	Point();
	Point(double x, double y);
	Point& operator= (Point&);
	void SetBoundary(int);
	void SetMu(double, double);
	double GetMuX();
	double GetMuY();
	void SetInternal(bool key);
	void SetTemp(double new_temp);
	double GetTemp();
	int GetBoundary();
	bool GetInternal();
};

