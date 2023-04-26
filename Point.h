#pragma once
class Point {
private:
	double x, y;
	double temp;
	double muX, muY;
	int boundary;
	bool internal;
	char g_u;
public:
	Point();
	Point(double x, double y);
	void SetBoundary(int key, double mx, double my);
	void SetInternal(bool key);
	void SetTemp(double new_temp);
	double GetTemp();
	int GetBoundary();
	bool GetInternal();
};

