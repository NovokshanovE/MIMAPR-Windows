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

void Object::PrintTempGrid(int k) {
	hole->Print();
	for (int i = 0; i < h_s; i++) {
		cout << endl;
		for (int j = 0; j < w_s; j++)
			if(temp_grid_[k][i][j].GetInternal())
				printf(" %03.f", temp_grid_[k][i][j].GetTemp());
			else
				printf(" ---");
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
			node.SetBoundary(BoundaryInit(j, i));
			node.SetMu(SetMuX(j, i, node.GetBoundary()), SetMuY(j, i, node.GetBoundary()));
			node.SetInternal(InternalSet(j, i));
			

			new_string.push_back(node);
		}
		new_grid.push_back(new_string);
	}
	temp_grid_.push_back(new_grid);

}
double Object::SetMuX(int x, int y, int key) {
	double res = 0;
	if (key) {
		if (x != 0 and x < w_s - 1) {
			
			if (InternalSet(x-1, y) and InternalSet(x, y-1)) {
				return 1;
			}
			else {
				if (hole->GetVariant()) {
					res = (abs(x * step - hole->GetX() + s) > abs(x * step - hole->GetX() - s)) ? abs(x * step - hole->GetX() - s) : abs(x * step - hole->GetX() + s);
					return (res < 1 / step) ? 1 : res / step;
				}
				else {
					if (x * step > hole->GetX()) {
						res = x * step - (hole->GetX() + sqrt(r1*r1 - (y - hole->GetY())* (y - hole->GetY())));
						return (res < 1 / step) ? 1 : res / step;
					}
					else {
						res = x * step - (hole->GetX() - sqrt(r1 * r1 - (y - hole->GetY()) * (y - hole->GetY())));
						return (res < 1 / step) ? 1 : res / step;
					}
				}
			}
		}
		else
			return 1;
	}
}
double Object::SetMuY(int x, int y, int key) {
	double res = 0;
	if (key) {
		if (y != 0 and y != h_s - 1) {
			if (InternalSet(x, y-1) and InternalSet(x-1, y)) {
				return 1;
			}
			else {
				if (hole->GetVariant()) {
					res = (abs(y * step - hole->GetY() + s) > abs(y * step - hole->GetY() - s)) ? abs(y * step - hole->GetY() - s) : abs(y * step - hole->GetY() + s);
					return (res < 1 / step) ? 1 : res / step;
				}
				else {
					if (y * step > hole->GetY()) {
						res = y * step - (hole->GetY() + sqrt(r1 * r1 - (x - hole->GetX()) * (x - hole->GetX())));
						return (res < 1 / step) ? 1 : res / step;
					}
					else {
						res = y * step - (hole->GetY() - sqrt(r1 * r1 - (x - hole->GetX()) * (x - hole->GetX())));
						return (res < 1/step) ? 1 : res / step;
					}
				}
			}
		}
		else
			return 1;
	}
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
	else {
		if (hole->GetVariant()) {
			if (x * step < hole->GetX() + step + s / 2 and (x * step > hole->GetX() - step - s / 2) and y * step < hole->GetY() + step + s / 2 and (y * step > hole->GetY() - step - s / 2))
				return boundary_[5];//3
		}
		else {
			if ((pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) < pow(r1 + step, 2) and (pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) < pow(r1 - step, 2))
				return boundary_[5];//3
		}
		return 0;
	}
}

Point& Object::PointOnNextStep(int x, int y, bool key, bool boundary) {

	if (key == false) {
		Point node_i_j = temp_grid_[time_ - 1][y][x];
		Point next_step_node = node_i_j;
		if (node_i_j.GetInternal()) {
			
			if (x != 0 and y != 0 and x != w_s - 1 and y != h_s - 1) {

				Point node_i_1_j = temp_grid_[time_ - 1][y + 1][x];
				Point node_1_i_j = temp_grid_[time_ - 1][y - 1][x];
				Point node_i_j_1 = temp_grid_[time_ - 1][y][x + 1];
				Point node_i__1_j = temp_grid_[time_ - 1][y][x - 1];
				//copy(node_i_1_j, temp_grid_[time_ - 1][y][x - 1]);

				double res = 0;
				switch (node_i_j.GetBoundary())
				{
				case 0:
					next_step_node.SetTemp(node_i_j.GetTemp() + h_t * (-4 * node_i_j.GetTemp() + node_1_i_j.GetTemp() + node_i_1_j.GetTemp() + node_i__1_j.GetTemp() + node_i_j_1.GetTemp()) / (step * step));
					break;
				case 1:
					if (node_i_j.GetMuX() == 1 and node_i_j.GetMuY() == 1) {
						next_step_node.SetTemp(100);
						return next_step_node;
					}
					else {
						if (temp_grid_[time_ - 1][y][x + 1].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + 100 + node_i_j.GetMuX() * node_i_j_1.GetTemp()) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						else if (temp_grid_[time_ - 1][y][x - 1].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + node_i_j.GetMuX() * node_i__1_j.GetTemp() + 100) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + 100 + node_i_j.GetMuY() * node_i_1_j.GetTemp()) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}

						else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + node_i_j.GetMuY() * node_1_i_j.GetTemp() + 100) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}
						next_step_node.SetTemp(res);
						return next_step_node;
					}
					
					break;
				case 2:
					if (node_i_j.GetMuX() == 1 and node_i_j.GetMuY() == 1) {
						next_step_node.SetTemp(200);
						return next_step_node;
					}
					else {
						if (temp_grid_[time_ - 1][y][x + 1].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + 200 + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x + 1].GetTemp()) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						else if (temp_grid_[time_ - 1][y][x - 1].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x - 1].GetTemp() + 200) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + 200 + node_i_j.GetMuY() * temp_grid_[time_ - 1][y + 1][x].GetTemp()) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}

						else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += node_i_j.GetTemp() + h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + node_i_j.GetMuY() * temp_grid_[time_ - 1][y - 1][x].GetTemp() + 200) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}
						next_step_node.SetTemp(res);
						return next_step_node;
					}
					
					break;

				case 3:
					if (boundary) {
						
						if (temp_grid_[time_ - 1][y][x + 1].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y][x + 1].GetTemp();
						}
						else if (temp_grid_[time_ - 1][y][x - 1].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y][x - 1].GetTemp();
						}
						if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y + 1][x].GetTemp();
						}
						
						else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y - 1][x].GetTemp();
						}
						next_step_node.SetTemp(res);
						return next_step_node;
						//if(node_i_j)
					}
					break;
				case 4:
					if (boundary) {
						if (x == 0) {
							res += temp_grid_[time_ - 1][y][x + 1].GetTemp() / (step * temp_grid_[time_ - 1][y][x + 1].GetMuX() + 1);
						}
						else if (y == 0) {
							res += temp_grid_[time_ - 1][y + 1][x].GetTemp() / (step * temp_grid_[time_ - 1][y + 1][x].GetMuY() + 1);
						}
						else if (x == w_s - 1) {
							res += temp_grid_[time_ - 1][y][x - 1].GetTemp() / (step * temp_grid_[time_ - 1][y][x - 1].GetMuX() + 1);
						}
						else if (y == h_s - 1) {
							res += temp_grid_[time_ - 1][y - 1][x].GetTemp() / (step * temp_grid_[time_ - 1][y - 1][x].GetMuY() + 1);
						}
						next_step_node.SetTemp(res);
						return next_step_node;
						//if(node_i_j)
					}
					break;

				default:
					break;
				}

			}
			else {
				switch (node_i_j.GetBoundary())
				{
				case 1:
					next_step_node.SetTemp(100);
					return next_step_node;
					break;
				case 2:
					next_step_node.SetTemp(200);
					return next_step_node;
					break;

				case 3:
					if (boundary) {
						if (x == 0) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y][x + 1].GetTemp());
						}
						else if (y == 0) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y + 1][x].GetTemp());
						}
						else if (x == w_s - 1) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y][x - 1].GetTemp());
						}
						else if (y == h_s - 1) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y - 1][x].GetTemp());
						}
						return next_step_node;
						//if(node_i_j)
					}
					break;
				case 4:
					if (boundary) {
						if (x == 0) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y][x + 1].GetTemp()/(step* temp_grid_[time_ - 1][y][x + 1].GetMuX()+1));
						}
						else if (y == 0) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y + 1][x].GetTemp() / (step * temp_grid_[time_ - 1][y + 1][x].GetMuX() + 1));
						}
						else if (x == w_s - 1) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y][x - 1].GetTemp() / (step * temp_grid_[time_ - 1][y][x - 1].GetMuX() + 1));
						}
						else if (y == h_s - 1) {
							next_step_node.SetTemp(temp_grid_[time_ - 1][y - 1][x].GetTemp() / (step * temp_grid_[time_ - 1][y - 1][x].GetMuX() + 1));
						}
						return next_step_node;
						//if(node_i_j)
					}
					break;

				default:
					break;
				}
			}
			
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

