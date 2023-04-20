#include <iostream>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include <cmath>
#include <vector>



using namespace Eigen;
using namespace std;
///usr/include/eigen3
vector <pair<double, double>> vec;
class Hole {
private:
	double X, Y;
	bool variant;
public:
	Hole(double X, double Y) {
		this->X = X;
		this->Y = Y;
	}
	double getX() {
		return X;
	}
	double getY() {
		return Y;
	}
	bool getVariant() {
		return variant;
	}
	void print() {
		cout << variant << ":" << X << " " << Y << endl;
	}
	void set_var(bool variant) {
		this->variant = variant;
	}
};

class Square : public Hole {
private:
	double S = 100;
public:
	Square(double p1, double p2) : Hole(p1, p2) {
		Hole::set_var(true);
	}
};

class Circle : public Hole {
private:
	double R = 50;
public:
	Circle(double p1, double p2) : Hole(p1, p2) {
		Hole::set_var(false);
	}
};

class Object {
private:
	int H = 400, W = 500;
	int S = 100;
	int R1 = 50, R2 = 150;
	int H_s, W_s;
	Hole* hole;
	int step = 10;
	ArrayXXf grid;
	vector<ArrayXXf*> temp_grid;
public:
	Object(int v, double p1, double p2) {
		if (v % 2)
			hole = new Square(p1, p2);
		else
			hole = new Circle(p1, p2);
		H_s = H / step + 1;
		W_s = W / step + 1;
		completion_grid();
	}
	void print() {
		hole->print();
		cout << grid << endl;
	}
	void completion_grid() {
		grid = ArrayXXf::Zero(H_s + 2, W_s + 2);
		//temp_grid = ArrayXXXf::Zero(H / step + 1, W / step + 1, 1);
		for (int y = 0; y < H_s; y++) {
			for (int x = 0; x < W_s; x++) {
				if (x * step >= 0 and (H_s - y - 1) * step >= 0 and x * step <= W and (H_s - y - 1) * step <= H) {
					if (hole->getVariant()) {
						if ((x * step > (W - R2)) and ((H_s - y - 1) * step > (H - R2)) and (pow((x * step - W + R2), 2) + pow(((H_s - y - 1) * step - H + R2), 2) > pow(R2, 2)))

							grid(y + 1, x + 1) = 0;
						else
							if (x * step < hole->getX() + S / 2 and (x * step > hole->getX() - S / 2) and y * step < hole->getY() + S / 2 and (y * step > hole->getY() - S / 2))
								grid(y + 1, x + 1) = 0;
							else
								grid(y + 1, x + 1) = 1;
					}
					else {
						if ((pow((x * step - hole->getX()), 2) + pow(((H_s - y - 1) * step - hole->getY()), 2)) < pow(R1, 2))
							grid(y + 1, x + 1) = 0;
						else
							if (x * step > W - R2 and (H_s - y - 1) * step > H - R2 and (pow((x * step - W + R2), 2) + pow(((H_s - y - 1) * step - H + R2), 2)) > pow(R2, 2))
								grid(y + 1, x + 1) = 0;
							else
								grid(y + 1, x + 1) = 1;
					}
				}
				else
					grid(y, x) = 0;
			}
		}
		//cout << grid(0, 0) << endl;

	}
	void completion_temp_grid() {
		ArrayXXf new_temp = ArrayXXf::Zero(H_s, W_s);

	}

};


int main()
{
	int variant;
	std::cin >> variant;
	int p1, p2;
	cin >> p1 >> p2;
	Object* body = new Object(variant, p1, p2);
	body->print();
	//ArrayXXf
	int in_put;
	cin >> in_put;
}