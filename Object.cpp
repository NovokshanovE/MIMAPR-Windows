#include "Object.h"

Object::Object(int v, double p1, double p2, int step, vector<int> boundary) {
	boundary_ = boundary;
	time_ = 0;
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
			node.SetBoundary(BoundaryInit(j, i), SetMuX(j, i), SetMuY(j, i));
			node.SetInternal(InternalSet(j, i));
			

			new_string.push_back(node);
		}
		new_grid.push_back(new_string);
	}
	temp_grid_.push_back(new_grid);

}
double Object::SetMuX(int x, int y) {

}
double Object::SetMuY(int x, int y) {

}
bool Object::InternalSet(double j, double i) {
	if (j * step >= 0 and i * step >= 0 and j * step <= w and i * step <= h) {

		if (hole->GetVariant()) {
			if ((j * step > (w - r2)) and (i * step > (h - r2)) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2) > pow(r2, 2))) {
				
				return false;
			}
			else
				if (j * step < hole->GetX() + s / 2 and (j * step > hole->GetX() - s / 2) and i * step < hole->GetY() + s / 2 and (i * step > hole->GetY() - s / 2)) {
					
					return false;
				}
				else {
					
					return true;
				}
		}
		else {
			if ((pow((j * step - hole->GetX()), 2) + pow((i * step - hole->GetY()), 2)) < pow(r1, 2)) {
				
				return false;
			}
			else
				if (j * step > (w - r2) and i * step > (h - r2) and (pow((j * step - w + r2), 2) + pow((i * step - h + r2), 2)) > pow(r2, 2)) {
					return false;
					
				}
				else {
					return true;
					
				}
		}
	}
	else
		return false;
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

Point& Object::PointOnNextStep(int x, int y, bool key, bool boundary) {
	if (key == false) {
		Point node_i_j = temp_grid_[time_ - 1][y][x];
		Point next_step_node = node_i_j;
		if (x != 0 and y != 0 and x != w_s - 1 and y != h_s - 1) {
			
			Point node_i_1_j = temp_grid_[time_ - 1][y + 1][x];
			Point node_1_i_j = temp_grid_[time_ - 1][y - 1][x];
			Point node_i_j_1 = temp_grid_[time_ - 1][y][x + 1];
			Point node_i__1_j = temp_grid_[time_ - 1][y][x - 1];
			//copy(node_i_1_j, temp_grid_[time_ - 1][y][x - 1]);
			

			switch (node_i_j.GetBoundary())
			{
			case 0:
				next_step_node.SetTemp(node_i_j.GetTemp() + h_t * (-4 * node_i_j.GetTemp() + node_1_i_j.GetTemp() + node_i_1_j.GetTemp() + node_i__1_j.GetTemp() + node_i_j_1.GetTemp()) / (step * step));
				break;
			case 1:
				next_step_node.SetTemp(100);
				break;
			case 2:
				next_step_node.SetTemp(200);
				break;

			case 3:
				if (boundary) {
					//if(node_i_j)
				}
				break;
			case 4:
				break;

			default:
				break;
			}
			
		}
		else {

		}
		return next_step_node;
		
		
	}
	else {

	}
	
}

void Object::NextStep(bool key) { 
	time_++;
	vector< vector<Point>> next_step_grid;
	//next_step_grid.reserve(h_s);
	for (int i = 0; i < h_s; i++) {
		vector<Point> next_string;// = next_step_grid[i];
		//next_string.reserve(w_s);
		for (int j = 0; j < w_s; j++) {

			Point node = PointOnNextStep(j, i, key, false);
			next_string.push_back(node);
			

			
		}
		next_step_grid.push_back(next_string);
	}
	for (int i = 0; i < h_s; i++) {
		for (int j = 0; j < w_s; j++) {
			if (temp_grid_[time_ - 1][i][j].GetBoundary() >= 3) {
				next_step_grid[i][j] = PointOnNextStep(j, i, key, true);
			}

		}
	}
	temp_grid_.push_back(next_step_grid);
}

