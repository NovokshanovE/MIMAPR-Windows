#include "Object.h"

Object::Object(int v, double p1, double p2, int step, vector<int> boundary) {
	boundary_ = boundary;
	if (v % 2)
		hole = new Square(p1, p2);
	else
		hole = new Circle(p1, p2);
	h_s = h / step + 1;
	w_s = w / step + 1;
	this->step = step;
	CompletionGrid();
	//CompletionTempGrid();
}
void Object::SetBoundary(vector<int> boundary) {
	boundary_ = boundary;
}

void Object::PrintGrid(int k) {
	hole->Print();
	for (int i = 0; i < h_s; i++) {
		cout << endl;
		for (int j = 0; j < w_s; j++)
			cout << ' ' << temp_grid_[k][i][j].GetBoundary();
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
					if ((j * step > (w - r2)) and (i * step > (h - r2)) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2) > pow(r2, 2))) {
						node.SetBoundary(BoundaryInit(j, i), 1, 1);
						node.SetInternal(false);
					}
					else
						if (j * step < hole->GetX() + s / 2 and (j * step > hole->GetX() - s / 2) and i * step < hole->GetY() + s / 2 and (i * step > hole->GetY() - s / 2)) {
							node.SetBoundary(BoundaryInit(j, i), 1, 1);
							node.SetInternal(false);
						}
						else {
							node.SetBoundary(BoundaryInit(j, i), 1, 1);
							node.SetInternal(true);
						}
				}
				else {
					if ((pow((j * step - hole->GetX()), 2) + pow((i * step - hole->GetY()), 2)) < pow(r1, 2)) {
						node.SetBoundary(BoundaryInit(j, i), 1, 1);
						node.SetInternal(false);
					}
					else
						if (j * step > (w - r2) and i * step > (h - r2) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2)) > pow(r2, 2)) {
							node.SetInternal(false);
							node.SetBoundary(BoundaryInit(j, i), 1, 1);
						}
						else {
							node.SetInternal(true);
							node.SetBoundary(BoundaryInit(j, i), 1, 1);
						}
				}
			}
			else
				node.SetInternal(false);

			new_string.push_back(node);
		}
		new_grid.push_back(new_string);
	}
	temp_grid_.push_back(new_grid);

}

int Object::BoundaryInit(double x, double y) {
	/*x *= step;
	y *= step;*/
	if (x * step < step and x * step > -step) {
		
		return boundary_[0];//4
	}
	else if (y * step < step and x * step > -step) {

		return boundary_[4];//3
	}
	else if (x * step > (w - r2) and y * step > (h - r2) and (pow((x * step - w + r2), 2) + pow((y * step - h + r2), 2)) > pow(r2 - step, 2)) {

		return boundary_[2];//3
	}
	else if (x * step < w + step and x * step > w - step) {

		return boundary_[3];//2
	}
	else if (y * step < h + step and y * step > h - step) {

		return boundary_[1];//1
	}
	else if (hole->GetVariant()) {
		if(x * step < hole->GetX()+step + s / 2 and (x * step > hole->GetX()- step - s / 2) and y * step < hole->GetY()+step + s / 2 and (y * step > hole->GetY()-step - s / 2))
			return boundary_[5];//3
	}
	else {
		if((pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) < pow(r1+step, 2) and (pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) < pow(r1 - step, 2))
			return boundary_[5];//3
	}
	return 0;
}

Point Object::PointOnNextStep(Point node) {

}

