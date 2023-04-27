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
	void SetBoundary(int key, double mx, double my);
	void SetInternal(bool key);
	void SetTemp(double new_temp);
	double GetTemp();
	int GetBoundary();
	bool GetInternal();
};

