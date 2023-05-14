#include "Object.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include<Eigen/SparseLU>
#include <math.h>
//#include <Eigen::SparseQR>
#include <io.h>

//#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)
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
			if(temp_grid_[k][i][j].GetInternal())
				cout << ' ' << temp_grid_[k][i][j].GetMuY();
			else
				cout << ' ' << "-";
	}
}
void Object::OutToFile() {
	//ofstream fout("output.txt");
	FILE* fd = fopen("output.txt", "w");
	fprintf(fd, "  t   x   y   T\n");
	for(int t = 0; t <= time_; t++)
		for (int i = 0; i < h_s; i++) {
		
			for (int j = 0; j < w_s; j++)
				if (temp_grid_[t][i][j].GetInternal()) {
					fprintf(fd, "%03d %03d %03d %-05.2f\n",t, j, i, temp_grid_[t][i][j].GetTemp());
					//fout << t << ' ' << j * step << i * step << temp_grid_[t][i][j].GetTemp();
				}
				/*else {
					fprintf(fd, "%03d %03d %03d %-05.2f\n", t, j, i, -100);
				}*/
			//fout << '\n';
			
				
		}
	fclose(fd);
	//fout.close();
}
void Object::PrintTempGrid(int k) {
	hole->Print();
	for (int i = 0; i < h_s; i++) {
		cout << endl;
		for (int j = 0; j < w_s; j++)
			if (temp_grid_[k][i][j].GetInternal()) {
				int t = temp_grid_[k][i][j].GetTemp()/29;
				//printf(" \033[9%dm#\033[0m", 7-t);
				printf(" %03.f", temp_grid_[k][i][j].GetTemp());
			}
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
			node.SetInternal(InternalSet(j, i));
			
			
			
			

			new_string.push_back(node);
		}
		new_grid.push_back(new_string);
	}
	temp_grid_.push_back(new_grid);

	for (int i = 0; i < h_s; i++) {
		
		//new_string.reserve(w_s);

		for (int j = 0; j < w_s; j++) {
			temp_grid_[0][i][j].SetBoundary(BoundaryInit(j, i, InternalSet(j, i)));
			double muX = SetMuX(j, i, temp_grid_[0][i][j].GetBoundary()), muY = SetMuY(j, i, temp_grid_[0][i][j].GetBoundary());
			vector<pair<int, int>> prov = { {0,1},{1,0},{0,-1},{-1,0} };
			
			if (muX < 0.01 or muY < 0.01) {
				temp_grid_[0][i][j].SetInternal(InternalSet(j, i));
				temp_grid_[0][i][j].SetBoundary(-1);
				for (int k = 0; k < 4; k++) {
					if (temp_grid_[0][i + prov[k].second][j + prov[k].first].GetInternal()) {
						temp_grid_[0][i + prov[k].second][j + prov[k].first].SetBoundary(BoundaryInit(j, i, InternalSet(j, i)));
						temp_grid_[0][i + prov[k].second][j + prov[k].first].SetMu(1, 1);
						

					}
				}
			}
			temp_grid_[0][i][j].SetMu(muX, muY);
		}
		
	}

}
double Object::SetMuX(int x, int y, int key) {
	double res = 0;
	if (key) {
		if (x != 0 and x < w_s - 1 and y != 0 and y < h_s - 1) {
			
			/*if (InternalSet(x-1, y) and InternalSet(x, y-1) ) {
				return 1;
			}*/
			//else {
			double res_r2 = abs(x * step - (w - r2 + sqrt(abs(r2 * r2 - (y * step - h + r2) * (y * step - h + r2)))));
			if (hole->GetVariant()) {
				res = (abs(x * step - (hole->GetX() + s/2)) > abs(x * step - (hole->GetX() - s/2))) ? abs(x * step - (hole->GetX() - s/2)) : abs(x * step - (hole->GetX() + s/2));
				res = (res < res_r2) ? res : res_r2;
				res = (res < step) ? res : step;
				return (res < 1 / step) ? 1 : (res / step);
			}
			else {
				if (x * step > hole->GetX()) {
					res = abs(x * step - (hole->GetX() + sqrt(abs(r1*r1 - (y * step - hole->GetY())* (y * step - hole->GetY())))));
					res = (res < res_r2) ? res : res_r2;
					res = (res < step) ? res : step;
					return (res < 1 / step) ? 1 : (res / step);
				}
				else {
					res = abs(x * step - (hole->GetX() - sqrt(abs(r1 * r1 - (y * step - hole->GetY()) * (y * step - hole->GetY())))));
					res = (res < res_r2) ? res : res_r2;
					res = (res < step) ? res : step;
					return (res < 1 / step) ? 1 : (res / step);
				}
			}
			//}
		}
		else
			return 1;
	}
	else
		return 1;
}
double Object::SetMuY(int x, int y, int key) {
	double res = 0;
	if (key) {
		if (x != 0 and x < w_s - 1 and y != 0 and y < h_s - 1) {
			/*if (InternalSet(x, y-1) and InternalSet(x-1, y)) {
				return 1;
			}
			else {*/
			double res_r2 = abs(y * step - (h - r2 + sqrt(abs(r2 * r2 - (x * step - w + r2) * (x * step - w + r2)))));
			if (hole->GetVariant()) {
				res = (abs(y * step - (hole->GetY() + s/2)) > abs(y * step - (hole->GetY() - s/2))) ? abs(y * step - (hole->GetY() - s/2)) : abs(y * step - (hole->GetY() + s/2));
				res = (res < res_r2) ? res : res_r2;
				res = (res < step) ? res : step;
				return (res < 1 / step) ? 1 : (res / step);
			}
			else {
				if (y * step > hole->GetY()) {
					res = y * step - (hole->GetY() + sqrt(abs(r1 * r1 - (x * step - hole->GetX()) * (x * step - hole->GetX()))));
					res = (res < res_r2) ? res : res_r2;
					res = (res < step) ? res : step;
					return (res < 1 / step) ? 1 : (res / step);
				}
				else {
					res = y * step - (hole->GetY() - sqrt(abs(r1 * r1 - (x * step - hole->GetX()) * (x * step - hole->GetX()))));
					res = (res < res_r2) ? res : res_r2;
					res = (res < step) ? res : step;
					return (res < 1 / step) ? 1 : (res / step);
				}
			}
			//}
		}
		else
			return 1;
	}
	else
		return 1;
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


int Object::BoundaryInit(double x, double y, bool key) {
	/*x *= step;
	y *= step;*/
	if (key) {
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
				//if(step == 10){
				if(temp_grid_[0][y-1][x].GetInternal() == 0 or temp_grid_[0][y][x-1].GetInternal() == 0 or temp_grid_[0][y + 1][x].GetInternal() == 0 or temp_grid_[0][y][x+1].GetInternal() == 0)
					//if ((pow((x * step - hole->GetX() - 2.5), 2) + pow((y * step - hole->GetY() - 2.5), 2)) < pow(r1 + sqrt(2) * step, 2) and (pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) >= pow(r1 - sqrt(2) * step, 2))
						return boundary_[5];//3
				//}
				//else {
				//	if ((pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) < pow(r1 + step, 2) and (pow((x * step - hole->GetX()), 2) + pow((y * step - hole->GetY()), 2)) >= pow(r1 - step, 2))
				//		return boundary_[5];
				//}

			}
			return 0;
		}
	}
	else {
		return -1;
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

				double res = node_i_j.GetTemp();
				switch (node_i_j.GetBoundary())
				{
				case 0:
					res += h_t * (-4 * node_i_j.GetTemp() + node_1_i_j.GetTemp() + node_i_1_j.GetTemp() + node_i__1_j.GetTemp() + node_i_j_1.GetTemp()) / (step * step);
					
					next_step_node.SetTemp(res);
					break;
				case 1:
					if (node_i_j.GetMuX() == 1 and node_i_j.GetMuY() == 1) {
						next_step_node.SetTemp(100);
						return next_step_node;
					}
					else {
						if (temp_grid_[time_ - 1][y][x + 1].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + 100 + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x + 1].GetTemp()) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
							
						}
						 else if (temp_grid_[time_ - 1][y][x - 1].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x - 1].GetTemp() + 100) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
							//printf("!");
						}
						//else {
						//	res += node_i_j.GetTemp() + h_t * (-2 * node_i_j.GetTemp() + temp_grid_[time_ - 1][y][x + 1].GetTemp() + temp_grid_[time_ - 1][y][x - 1].GetTemp()) / (step * step);
						//}
						if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + 100 + node_i_j.GetMuY() * temp_grid_[time_ - 1][y + 1][x].GetTemp()) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}

						 else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + node_i_j.GetMuY() * temp_grid_[time_ - 1][y - 1][x].GetTemp() + 100) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
							//printf("!");
						}
						//else {
						//	res += node_i_j.GetTemp() + h_t * (-2 * node_i_j.GetTemp() + temp_grid_[time_ - 1][y + 1][x].GetTemp() + temp_grid_[time_ - 1][y - 1][x].GetTemp()) / (step * step);
						//}
						
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
							res += h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + 200 + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x + 1].GetTemp()) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						else if (temp_grid_[time_ - 1][y][x - 1].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuX()) * node_i_j.GetTemp() + node_i_j.GetMuX() * temp_grid_[time_ - 1][y][x - 1].GetTemp() + 200) / (step * step * (node_i_j.GetMuX() * node_i_j.GetMuX() + node_i_j.GetMuX()));
						}
						if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + 200 + node_i_j.GetMuY() * temp_grid_[time_ - 1][y + 1][x].GetTemp()) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
						}

						else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += h_t * 2 * (-(1 + node_i_j.GetMuY()) * node_i_j.GetTemp() + node_i_j.GetMuY() * temp_grid_[time_ - 1][y - 1][x].GetTemp() + 200) / (step * step * (node_i_j.GetMuY() * node_i_j.GetMuY() + node_i_j.GetMuY()));
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
						else if (temp_grid_[time_ - 1][y + 1][x].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y + 1][x].GetTemp();
						}
						
						else if (temp_grid_[time_ - 1][y - 1][x].GetBoundary() == 0) {
							res += temp_grid_[time_ - 1][y - 1][x].GetTemp();
						}
						res = 0;
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
						next_step_node.SetTemp(0);
						/*if (x == 0) {
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
						}*/
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
	if (key == false) {
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
	else if(key == true){
		time_ ++;
		double mu;
		double difw = 0;
		double difr = 0;
		double** ws;
		ws = new double* [h_s];
		int size_matr;
		
		for (int i = 0; i < h_s; i++) {
			ws[i] = new double[w_s];
			size_matr = 0;
			for (int j = 0; j < w_s; j++)
				if (temp_grid_[time_ - 1][i][j].GetInternal())
					size_matr++;
			if (size_matr == 0)
				continue;
			Eigen::SparseMatrix<double> sm(size_matr, size_matr);
			Eigen::VectorXd res(size_matr), x(size_matr);
			size_matr = 0;
			int last_s = 1000;
			for (int j = 0; j < w_s; j++)
			{
				if (temp_grid_[time_ - 1][i][j].GetInternal())
				{
					mu = temp_grid_[time_ - 1][i][j].GetMuX();
					difw = 0;
					difr = 0;

					if (temp_grid_[time_ - 1][i][j].GetBoundary())
					{
						switch (temp_grid_[time_ - 1][i][j].GetBoundary())
						{
						case 1:
							difr = -(100 * 2 * h_t / (step * step * mu * (1 + mu)));
							difw = 0;
							break;
						case 2:
							difr = -(200 * 2 * h_t / (step * step * mu * (1 + mu)));
							difw = 0;
							break;
						case 3:
							difr = 0;
							difw = 2 * h_t / (step * step * mu * (1 + mu));
							break;
						case 4:
							difr = 0;
							difw = 2 * h_t / (step * step * mu * (1 + mu) * (1 + step * mu));
							break;
						}

					}
					//cout << "difr:" << endl;
					if (j != w_s - 1 and (temp_grid_[time_ - 1][i][j].GetBoundary() != 0 and  temp_grid_[time_ - 1][i][j + 1].GetBoundary() == 0) or temp_grid_[time_ - 1][i][j].GetBoundary() == 0) {
						if (size_matr + 1 > last_s) {
							cout << endl;
						}
						sm.insert(size_matr, size_matr + 1) = 2 * h_t / (step * step * (1 + mu));  // ”множение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
					}
					if (j != 0 and (temp_grid_[time_ - 1][i][j].GetBoundary() != 0 and  temp_grid_[time_ - 1][i][j - 1].GetBoundary() == 0) or temp_grid_[time_ - 1][i][j].GetBoundary() == 0)
					{
						if (size_matr - 1 < 0) {
							cout << endl;
						}
						sm.insert(size_matr, size_matr - 1) = 2 * h_t / (step * step * (1 + mu));  // ”множение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
					}
					//cout << "mu:" << mu << endl;
					sm.insert(size_matr, size_matr) = -(2 * h_t / (mu * step * step) + 1) + difw;
					res(size_matr) = -temp_grid_[time_ - 1][i][j].GetTemp() + difr;
					size_matr++;
				}
			}
			last_s = size_matr;
			if (size_matr > 0)
			{
				Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
				//cout << sm.size() << endl;
				//cout << sm << endl;
				solver.compute(sm);
				//cout << (solver.info() == Eigen::Success) << endl;
				//cout << res.size() << endl;
				x = solver.solve(res);
				size_matr = 0;
				for (int j = 0; j < w_s; j++)
				{
					if (temp_grid_[time_ - 1][i][j].GetInternal())
					{
						ws[i][j] = x(size_matr);
						size_matr++;
					}
				}
			}
		}
//-------------------------------------------------------------------------------------------------
		vector< vector<Point>> next_step_grid;
		
		for (int i = 0; i < h_s; i++) {
			vector<Point> next_string;
			for (int j = 0; j < w_s; j++) {
				Point next_step_node = temp_grid_[time_ - 1][i][j];
				next_string.push_back(next_step_node);
			}
			next_step_grid.push_back(next_string);
		}
		temp_grid_.push_back(next_step_grid);
		for (int j = 0; j < w_s; j++) {
			
			//ws[i] = new double[w_s];
			size_matr = 0;
			for (int i = 0; i < h_s; i++)
				if (temp_grid_[time_ - 1][i][j].GetInternal())
					size_matr++;
			Eigen::SparseMatrix<double> sm(size_matr, size_matr);
			Eigen::VectorXd res(size_matr), x(size_matr);
			size_matr = 0;
			for (int i = 0; i < h_s; i++)
			{
				if (temp_grid_[time_ - 1][i][j].GetInternal())
				{
					mu = temp_grid_[time_ - 1][i][j].GetMuY();
					difw = 0;
					difr = 0;

					if (temp_grid_[time_ - 1][i][j].GetBoundary())
					{
						switch (temp_grid_[time_ - 1][i][j].GetBoundary())
						{
						case 1:
							difr = -(100 * 2 * h_t / (step * step * mu * (1 + mu)));
							difw = 0;
							break;
						case 2:
							difr = -(200 * 2 * h_t / (step * step * mu * (1 + mu)));
							difw = 0;
							break;
						case 3:
							difr = 0;
							difw = 2 * h_t / (step * step * mu * (1 + mu));
							break;
						case 4:
							difr = 0;
							difw = 2 * h_t / (step * step * mu * (1 + mu) * (1 + step * mu));
							break;
						}

					}
					if (temp_grid_[time_ - 1][i][j].GetBoundary() != 0 and (i != h_s - 1 and temp_grid_[time_ - 1][i+1][j].GetBoundary() == 0) or temp_grid_[time_ - 1][i][j].GetBoundary() == 0) {

						sm.insert(size_matr, size_matr + 1) = 2 * h_t / (step * step * (1 + mu));  // ”множение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
					}
					if (temp_grid_[time_ - 1][i][j].GetBoundary() != 0 and (i != 0 and temp_grid_[time_ - 1][i-1][j].GetBoundary() == 0) or temp_grid_[time_ - 1][i][j].GetBoundary() == 0 )
					{
						sm.insert(size_matr, size_matr - 1) = 2 * h_t / (step * step * (1 + mu));  // ”множение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
					}
					sm.insert(size_matr, size_matr) = -(2 * h_t / (mu * step * step) + 1) + difw;
					res(size_matr) = -temp_grid_[time_ - 1][i][j].GetTemp() + difr;
					size_matr++;
				}
			}
			if (size_matr > 0)
			{
				Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
				solver.compute(sm);
				x = solver.solve(res);
				size_matr = 0;
				for (int i = 0; i < h_s; i++)
				{
					
					if (temp_grid_[time_ - 1][i][j].GetInternal())
					{
						temp_grid_[time_][i][j].SetTemp(x(size_matr));
						size_matr++;
					}
					
				}
			}
			

		}
		
		//for (int i = 0; i < edges.size(); i++)
			//edges[i]->calculate(t);
		
	}
		
	
}

