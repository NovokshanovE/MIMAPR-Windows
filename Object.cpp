#include "Object.h"

Object::Object(int v, double p1, double p2) {
	if (v % 2)
		hole = new Square(p1, p2);
	else
		hole = new Circle(p1, p2);
	h_s = h / step + 1;
	w_s = w / step + 1;
	CompletionGrid();
	//CompletionTempGrid();
}

void Object::PrintGrid() {
	hole->Print();
	//cout << grid << endl;
}

void Object::CompletionGrid() {
	//grid = ArrayXXf::Zero(h_s + 0, w_s + 0);
	////temp_grid = ArrayXXXf::Zero(H / step + 1, w / step + 1, 1);
	//for (int y = 0; y < h_s; y++) {
	//	for (int x = 0; x < w_s; x++) {
	//		if (x * step >= 0 and y * step >= 0 and x * step <= w and y * step <= H) {
	//			if (hole->getVariant()) {
	//				if ((x * step > (W - R2)) and (y * step > (H - R2)) and (pow((x * step - w + R2), 2) + pow((y * step - h + R2), 2) > pow(R2, 2)))

	//					grid(y, x) = 0;
	//				else
	//					if (x * step < hole->getX() + S / 2 and (x * step > hole->getX() - S / 2) and y * step < hole->getY() + S / 2 and (y * step > hole->getY() - S / 2))
	//						grid(y, x) = 0;
	//					else
	//						grid(y, x) = 1;
	//			}
	//			else {
	//				if ((pow((x * step - hole->getX()), 2) + pow((y * step - hole->getY()), 2)) < pow(R1, 2))
	//					grid(y, x) = 0;
	//				else
	//					if (x * step > w - R2 and y * step > h - R2 and (pow((x * step - w + R2), 2) + pow((y * step - h + R2), 2)) > pow(R2, 2))
	//						grid(y, x) = 0;
	//					else
	//						grid(y, x) = 1;
	//			}
	//		}
	//		else
	//			grid(y, x) = 0;
	//	}
	//}
	//cout << grid(0, 0) << endl;

}
void Object::CompletionTempGrid() {

	//for (int i = 0; i < h_s; i++) {
	//	for (int j = 0; j < w_s; j++) {
	//		if (grid(i, j)) {
	//			if (j == 0) {
	//				//new_temp(i, j) = 100;

	//			}
	//			else if (j == w_s - 1) {


	//			}
	//		}
	//	}
	//}
	////temp_grid.push_back();



}