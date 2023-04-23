#include "Object.h"

Object::Object(int v, double p1, double p2) {
	if (v % 2)
		hole = new Square(p1, p2);
	else
		hole = new Circle(p1, p2);
	H_s = H / step + 1;
	W_s = W / step + 1;
	completion_grid();
	completion_temp_grid();
}

void Object::print() {
	hole->print();
	cout << grid << endl;
}

void Object::completion_grid() {
	grid = ArrayXXf::Zero(H_s + 0, W_s + 0);
	//temp_grid = ArrayXXXf::Zero(H / step + 1, W / step + 1, 1);
	for (int y = 0; y < H_s; y++) {
		for (int x = 0; x < W_s; x++) {
			if (x * step >= 0 and y * step >= 0 and x * step <= W and y * step <= H) {
				if (hole->getVariant()) {
					if ((x * step > (W - R2)) and (y * step > (H - R2)) and (pow((x * step - W + R2), 2) + pow((y * step - H + R2), 2) > pow(R2, 2)))

						grid(y, x) = 0;
					else
						if (x * step < hole->getX() + S / 2 and (x * step > hole->getX() - S / 2) and y * step < hole->getY() + S / 2 and (y * step > hole->getY() - S / 2))
							grid(y, x) = 0;
						else
							grid(y, x) = 1;
				}
				else {
					if ((pow((x * step - hole->getX()), 2) + pow((y * step - hole->getY()), 2)) < pow(R1, 2))
						grid(y, x) = 0;
					else
						if (x * step > W - R2 and y * step > H - R2 and (pow((x * step - W + R2), 2) + pow((y * step - H + R2), 2)) > pow(R2, 2))
							grid(y, x) = 0;
						else
							grid(y, x) = 1;
				}
			}
			else
				grid(y, x) = 0;
		}
	}
	//cout << grid(0, 0) << endl;

}
void Object::completion_temp_grid() {

	for (int i = 0; i < H_s; i++) {
		for (int j = 0; j < W_s; j++) {
			if (grid(i, j)) {
				if (j == 0) {
					//new_temp(i, j) = 100;

				}
				else if (j == W_s - 1) {


				}
			}
		}
	}
	//temp_grid.push_back();



}