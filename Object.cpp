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

void Object::PrintGrid(int k) {
	hole->Print();
	for (int i = 0; i < h_s; i++) {
		cout << endl;
		for (int j = 0; j < w_s; j++)
			cout << ' ' << temp_grid_[k][i][j].GetInternal();
	}
		

		
	
}


void Object::CompletionGrid() {
	vector< vector <Point>> new_grid;
	//new_grid.reserve(h_s);
	
	for (int i = 0; i < h_s; i++) {
		vector <Point> new_string;
		//new_string.reserve(w_s);

		for (int j = 0; j < w_s; j++) {
			Point node(j * step, i * step);
			if (j * step >= 0 and i * step >= 0 and j * step <= w and i * step <= h) {
				
				if (hole->GetVariant()) {
					if ((j * step > (w - r2)) and (i * step > (h - r2)) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2) > pow(r2, 2)))

						node.SetInternal(false);
					else
						if (j * step < hole->GetX() + s / 2 and (j * step > hole->GetX() - s / 2) and i * step < hole->GetY() + s / 2 and (i * step > hole->GetY() - s / 2))
							node.SetInternal(false);
						else
							node.SetInternal(true);
				}
				else {
					if ((pow((j * step - hole->GetX()), 2) + pow((i * step - hole->GetY()), 2)) < pow(r1, 2))
						node.SetInternal(false);
					else
						if (j * step > (w - r2) and i * step > (h - r2) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2)) > pow(r2, 2))
							node.SetInternal(false);
						else
							node.SetInternal(true);
				}
			}
			else
				node.SetInternal(false);

			new_string.push_back(node);
		}
		new_grid.push_back(new_string);
	}
	temp_grid_.push_back(new_grid);
	//grid = ArrayXXf::Zero(h_s + 0, w_s + 0);
	////temp_grid = ArrayXXXf::Zero(H / step + 1, w / step + 1, 1);
	//for (int y = 0; y < h_s; y++) {
	//	for (int x = 0; x < w_s; x++) {
	//		
	//	}
	//}
	//cout << grid(0, 0) << endl;

}
