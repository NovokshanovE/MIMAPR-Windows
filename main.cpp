#include <vector>
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include<Eigen/SparseLU>

using namespace std;

class Point
{
public:
	double x, y;
	Point(double x_, double y_) : x(x_), y(y_) {}
	Point() : x(0), y(0) {}
};

class EdgeCondition
{
public:
	int type;
	double T;
	EdgeCondition(int type_, double T_ = 0) : type(type_), T(T_) {}
};

struct Form
{
private:
	EdgeCondition cond;
public:
	virtual bool is_inside(double x, double y, bool is_add) = 0;
	virtual double get_dist_x(double x, double y) = 0;
	virtual double get_dist_y(double x, double y) = 0;
	virtual char get_form() = 0;

	virtual Point get_p1() = 0;
	virtual Point get_p2() = 0;

	Form(EdgeCondition c) : cond(c) {}
	EdgeCondition get_cond() { return cond; }
};

struct Rectangle : Form
{
	Point p1, p2;
public:
	Rectangle(double x1, double y1, double x2, double y2, EdgeCondition c) : Form(c)
	{
		p1.x = min(x1, x2);
		p1.y = min(y1, y2);
		p2.x = max(x1, x2);
		p2.y = max(y1, y2);
	}
	bool is_inside(double x, double y, bool is_add)
	{
		if (is_add)
			return x > p1.x && x < p2.x&& y > p1.y && y < p2.y;
		else
			return x >= p1.x && x <= p2.x&& y >= p1.y && y <= p2.y;
	}
	double get_dist_x(double x, double y)  // Надо перевернуть обратно, возможно у тоже и просмотреть их использование
	{
		if (abs(x - p1.x) < abs(x - p2.x))
			return p1.x - x;
		else
			return p2.x - x;
	}
	double get_dist_y(double x, double y)
	{
		if (abs(y - p1.y) < abs(y - p2.y))
		{
			//cout << p1.y - y << endl;
			return p1.y - y;
		}
		else
		{
			//cout << p2.y - y << endl;
			return p2.y - y;
		}
	}
	char get_form() { return 'r'; }
	Point get_p1() { return p1; }
	Point get_p2() { return p2; }
};

struct Circle : Form
{
	double r;
	Point o;
public:
	Circle(double x, double y, double r_, EdgeCondition c) : Form(c), o(Point(x, y)), r(r_) {}
	bool is_inside(double x, double y, bool is_add)
	{
		if (is_add)
			return (o.x - x) * (o.x - x) + (o.y - y) * (o.y - y) < r * r;
		else
			return (o.x - x) * (o.x - x) + (o.y - y) * (o.y - y) <= r * r;
	}
	double get_dist_x(double x, double y)
	{
		if (abs(-x + o.x - sqrt(abs(r * r - (y - o.y) * (y - o.y)))) < abs(-x + o.x + sqrt(abs(r * r - (y - o.y) * (y - o.y)))))
			return -x + o.x - sqrt(abs(r * r - (y - o.y) * (y - o.y)));
		else
			return -x + o.x + sqrt(abs(r * r - (y - o.y) * (y - o.y)));
	}
	double get_dist_y(double x, double y)
	{
		if (abs(o.y - sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y) < abs(o.y + sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y))
		{
			//cout << o.y - sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y << endl;
			return o.y - sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y;
		}
		else
		{
			//cout << o.y + sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y << endl;
			return o.y + sqrt(abs(r * r - (x - o.x) * (x - o.x))) - y;
		}
	}
	char get_form() { return 'c'; }
	Point get_p1() { return Point(o.x - r, o.y - r); }
	Point get_p2() { return Point(o.x + r, o.y + r); }
};

class Element;

class EdgeElement : public Point  // Должны быть привязаны к нулям форм, которые их породили
{
	// Здесь x и y будут реальными для возможной отрисовки, потому что в сетку могут не попасть
	double *T;
	double mu;
	double h, h_t;
	Element* parent;
public:
	EdgeCondition cond;
	EdgeElement(Element *p_, double x, double y, EdgeCondition c, double mu_, int sim_t, double h_, double h_t_, double T_ = 0) :
		parent(p_), cond(c), Point(x, y), mu(mu_), h(h_), h_t(h_t_)
	{
		T = new double[sim_t + 1];
		T[0] = T_;
	}
	void get_edge_dif(double* difw, double* difr)
	{
		switch (cond.type)
		{
		case 3:
			*difr = -(cond.T * 2 * h_t / (h * h * mu * (1 + mu)));
			*difw = 0;
			break;
		case 4:
			*difr = cond.T * 2 * h_t / (h * (1 + mu));
			*difw = 2 * h_t / (h * h * mu * (1 + mu));
			break;
		case 5:
			*difr = 0;
			*difw = 2 * h_t / (h * h * mu * (1 + mu) * (1 + h * mu));
			break;
		}
	}
	double get_mu() { return mu; }
	void calculate(int t);
	double get_T(int t) { return T[t]; }
};

class Element : public Point
{
	bool active;
	Form* parent;
public:
	double* T;
	EdgeElement* edge_x;
	EdgeElement* edge_y;
	Element(double x, double y, double T_, Form *p, bool active_, int sim_t) : Point(x, y), parent(p), edge_x(NULL), edge_y(NULL), active(active_)
	{
		T = new double[sim_t + 1];
		T[0] = T_;
	}
	bool is_active() { return active; }
	void set_active(bool a) { active = a; }
Form* get_parent() { return parent; }
void set_parent(Form* p) { parent = p; }
double get_T(int t) { return T[t]; }
};

void EdgeElement::calculate(int t)
{
	switch (cond.type)
	{
	case 3:
		T[t] = cond.T;
		if (T[t] > 200)
		{
			cout << parent->x << " " << parent->y << " ";
			cout << T[t] << endl;
		}
		break;
	case 4:
		T[t] = parent->get_T(t) - mu * h * cond.T;
		break;
	case 5:
		if (abs(mu) < 0.00001)
		{
			cout << "\n\nERROR!\n\n";
			exit(0);
		}
		T[t] = (mu * parent->get_T(t) / (mu * h + 1));
		break;
	default:
		cout << "\n\nERROR!\n\n";
		exit(0);
		break;
	}
}

class Object  // Не забыть добавить деструктор с очисткой памяти
{
private:
	vector<Form*> forms;
	int n, m;
	int sim_t;
	double real_n, real_m;
	double h, h_t;
	bool** bin_mask;
	Element*** elements;
	vector<EdgeElement*> edges;
public:
	Object(double real_n_, double real_m_, double h_, double sim_t_, double h_t_) :
		n(real_n_ / h_ + 1), m(real_m_ / h_ + 1), real_n(real_n_), real_m(real_m_), h(h_), sim_t(sim_t_ / h_t_), h_t(h_t_)
	{
		bin_mask = new bool* [m];
		elements = new Element * *[m];
		for (int i = 0; i < m; i++)
		{
			bin_mask[i] = new bool[n];
			elements[i] = new Element * [n];
			for (int j = 0; j < n; j++)
			{
				bin_mask[i][j] = 0;
				elements[i][j] = NULL;
			}
		}

	}
	void add_form(Form* f, double NU, bool is_add)
	{
		forms.push_back(f);
		for (int i = (f->get_p1()).y / h; i <= (f->get_p2()).y / h; i += 1)
			for (int j = (f->get_p1()).x / h; j <= (f->get_p2()).x / h; j += 1)
			{
				if (i >= 0 && i * h <= real_m && j >= 0 && j * h <= real_n && f->is_inside(j * h, i * h, is_add))
				{
					if (elements[i][j])
						delete elements[i][j];
					elements[i][j] = new Element(j, i, NU, f, is_add, sim_t);
					bin_mask[i][j] = is_add;
				}
			}
	}

	void compile_edges()
	{
		Form* p;
		double dist, mu;
		int di[4] = { -1, 0, 0, 1 }, dj[4] = { 0, 1, -1, 0 };
		for (int i = 0; i < m; i++)  // Удаление мелких мю, превращение их в дырки с граничным условием как у детали, из которой удалили
			for (int j = 0; j < n; j++)
			{
				if (!elements[i][j] || !elements[i][j]->is_active())
				{
					for (int k = 0; k < 4; k++)
					{
						if (i + di[k] < 0 || i + di[k] >= m || j + dj[k] < 0 || j + dj[k] >= n)
							continue;
						if (bin_mask[i + di[k]][j + dj[k]])
						{
							if (elements[i][j] && !elements[i][j]->is_active())
								p = elements[i][j]->get_parent();
							else
								p = elements[i + di[k]][j + dj[k]]->get_parent();
							dist = min(abs(p->get_dist_x((j + dj[k]) * h, (i + di[k]) * h)), abs(p->get_dist_y((j + dj[k]) * h, (i + di[k]) * h)));
							if (dist <= 1 / h + 0.0000001 && dist >= 0.0000001)
							{
								//elements[i + di[k]][j + dj[k]]->set_parent(p);
								elements[i + di[k]][j + dj[k]]->set_active(0);
							}
						}
					}
					
				}
			}
		//*/
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
			{
				if (!elements[i][j] || !elements[i][j]->is_active())
				{
					if (i != 0 && bin_mask[i - 1][j] && elements[i - 1][j]->is_active())
					{
						if (elements[i][j] && !elements[i][j]->is_active())
							p = elements[i][j]->get_parent();
						else
							p = elements[i - 1][j]->get_parent();
						if (bin_mask[i][j])
							dist = h;
						else
							dist = p->get_dist_y(j * h, (i - 1) * h);
						
						edges.push_back(new EdgeElement(elements[i - 1][j], j * h, (i - 1) * h + dist, p->get_cond(), abs(dist / h), sim_t, h, h_t));
						elements[i - 1][j]->edge_y = edges[edges.size() - 1];
					}
					if (i != m - 1 && bin_mask[i + 1][j] && elements[i + 1][j]->is_active())
					{
						if (elements[i][j] && !elements[i][j]->is_active())
							p = elements[i][j]->get_parent();
						else
							p = elements[i + 1][j]->get_parent();
						if (bin_mask[i][j])
							dist = h;
						else
							dist = p->get_dist_y(j * h, (i + 1) * h);
						
						//Проверка что грань совпадает у 2-х узлов, но это надо для 2х направлений (x-1 и y-1) в каждом из 4х направлений, лень
						if (j != 0 && bin_mask[i][j - 1] && elements[i][j - 1]->edge_x && (elements[i][j - 1]->edge_x)->x == j * h
							&& (elements[i][j - 1]->edge_x)->y == (i + 1) * h + dist)
							elements[i + 1][j]->edge_y = elements[i][j - 1]->edge_x;
						else if (j != n - 1 && bin_mask[i][j + 1] && elements[i][j + 1]->edge_x && (elements[i][j + 1]->edge_x)->x == j * h
							&& (elements[i][j + 1]->edge_x)->y == (i + 1) * h + dist)
							elements[i + 1][j]->edge_y = elements[i][j + 1]->edge_x;
						else
						{
							edges.push_back(new EdgeElement(elements[i + 1][j], j * h, (i + 1) * h + dist, p->get_cond(), abs(dist / h), sim_t, h, h_t));
							elements[i + 1][j]->edge_y = edges[edges.size() - 1];
						}
					}
					if (j != 0 && bin_mask[i][j - 1] && elements[i][j - 1]->is_active())
					{
						if (elements[i][j] && !elements[i][j]->is_active())
							p = elements[i][j]->get_parent();
						else
							p = elements[i][j - 1]->get_parent();
						if (bin_mask[i][j])
							dist = h;
						else
							dist = p->get_dist_x((j - 1) * h, i * h);
						
						//Проверка что грань совпадает у 2-х узлов, но это надо для 2х направлений (x-1 и y-1) в каждом из 4х направлений, лень
						if (i != 0 && bin_mask[i - 1][j] && elements[i - 1][j]->edge_y && (elements[i - 1][j]->edge_y)->y == i * h
							&& (elements[i - 1][j]->edge_y)->x == (j - 1) * h + dist)
							elements[i][j - 1]->edge_x = elements[i - 1][j]->edge_y;
						else
						{
							edges.push_back(new EdgeElement(elements[i][j - 1], (j - 1) * h + dist, i * h, p->get_cond(), abs(dist / h), sim_t, h, h_t));
							elements[i][j - 1]->edge_x = edges[edges.size() - 1];
						}
					}
					if (j != n - 1 && bin_mask[i][j + 1] && elements[i][j + 1]->is_active())
					{
						if (elements[i][j] && !elements[i][j]->is_active())
							p = elements[i][j]->get_parent();
						else
							p = elements[i][j + 1]->get_parent();
						if (bin_mask[i][j])
							dist = h;
						else
							dist = p->get_dist_x((j + 1) * h, i * h);
						
						if (i != 0 && bin_mask[i - 1][j] && elements[i - 1][j]->edge_y && (elements[i - 1][j]->edge_y)->y == i * h
							&& (elements[i - 1][j]->edge_y)->x == (j + 1) * h + dist)
							elements[i][j + 1]->edge_x = elements[i - 1][j]->edge_y;
						else
						{
							edges.push_back(new EdgeElement(elements[i][j + 1], (j + 1) * h + dist, i * h, p->get_cond(), abs(dist / h), sim_t, h, h_t));
							elements[i][j + 1]->edge_x = edges[edges.size() - 1];
						}
					}
				}
			}
	}
	void calculate_explicit()  // Не работает если мю с обоих сторон
	{
		for (int i = 0; i < edges.size(); i++)
			edges[i]->calculate(0);
		for (int t = 1; t <= sim_t; t++)
		{
			for (int i = 0; i < m; i++)
				for (int j = 0; j < n; j++)
					if (elements[i][j] && elements[i][j]->is_active())
					{
						elements[i][j]->T[t] = 0;
						EdgeElement* ex = elements[i][j]->edge_x;
						EdgeElement* ey = elements[i][j]->edge_y;
						double sum = elements[i][j]->T[t - 1];
						if (ex && ex->x < (elements[i][j]->x) * h)
							sum += h_t * 2 * ((ex->get_mu() * elements[i][j + 1]->T[t - 1] - (ex->get_mu() + 1) * elements[i][j]->T[t - 1] + ex->get_T(t - 1))
								/ (h * h * ex->get_mu() * (ex->get_mu() + 1)));
							//sum += 1;
						else if (ex)
							sum += h_t * 2 * ((ex->get_mu() * elements[i][j - 1]->T[t - 1] - (ex->get_mu() + 1) * elements[i][j]->T[t - 1] + ex->get_T(t - 1))
								/ (h * h * ex->get_mu() * (ex->get_mu() + 1)));
							//sum += 1;
						else
							sum += h_t * ((elements[i][j - 1]->T[t - 1] - 2 * elements[i][j]->T[t - 1] + elements[i][j + 1]->T[t - 1])
								/ (h * h));
							//sum += 1;
						if (ey && ey->y < (elements[i][j]->y) * h)
							sum += h_t * (2 * ((ey->get_mu()) * (elements[i + 1][j]->T[t - 1]) - ((ey->get_mu()) + 1) * (elements[i][j]->T[t - 1]) + (ey->get_T(t - 1)))
								/ (h * h * (ey->get_mu()) * ((ey->get_mu()) + 1)));
							//sum += 1;
						else if (ey)
							sum += h_t * (2 * ((ey->get_mu()) * (elements[i - 1][j]->T[t - 1]) - ((ey->get_mu()) + 1) * (elements[i][j]->T[t - 1]) + (ey->get_T(t - 1)))
								/ (h * h * (ey->get_mu()) * ((ey->get_mu()) + 1)));
							//sum += 0;
						else
							sum += h_t * ((elements[i - 1][j]->T[t - 1] - 2 * elements[i][j]->T[t - 1] + elements[i + 1][j]->T[t - 1])
								/ (h * h));
							//sum += 1;
						elements[i][j]->T[t] = sum;
					}
			for (int i = 0; i < edges.size(); i++)
				edges[i]->calculate(t);
		}
	}

	void calculate_implicit()
	{
		double mu;
		double* difw = new double(0);
		double* difr = new double(0);
		double** ws;
		ws = new double* [m];
		int size_matr;
		for (int t = 1; t <= sim_t; t++)
		{
			for (int i = 0; i < m; i++)
			{
				ws[i] = new double[n];
				size_matr = 0;
				for (int j = 0; j < n; j++)
					if (elements[i][j] && elements[i][j]->is_active())
						size_matr++;
				Eigen::SparseMatrix<double> sm(size_matr, size_matr);
				Eigen::VectorXd res(size_matr), x(size_matr);
				size_matr = 0;
				for (int j = 0; j < n; j++)
				{
					if (elements[i][j] && elements[i][j]->is_active())
					{
						mu = 1;
						*difw = 0;
						*difr = 0;
						EdgeElement* ex = elements[i][j]->edge_x;
						if (ex)
						{
							ex->get_edge_dif(difw, difr);
							mu = ex->get_mu();
						}
						if (!ex || (ex && ex->x < elements[i][j]->x))
						{
							sm.insert(size_matr, size_matr + 1) = 2 * h_t / (h * h * (1 + mu));  // Умножение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
						}
						if (!ex || (ex && ex->x >= elements[i][j]->x))
						{
							sm.insert(size_matr, size_matr - 1) = 2 * h_t / (h * h * (1 + mu));  // Умножение на мю есть, т. к. если не конечный, то пох, иначе всегда с мю
						}
						sm.insert(size_matr, size_matr) = -(2 * h_t / (mu * h * h) + 1) + *difw;
						res(size_matr) = -elements[i][j]->get_T(t - 1) + *difr;
						size_matr++;
					}
				}
				if (size_matr > 0)
				{
					Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
					solver.compute(sm);
					x = solver.solve(res);
					size_matr = 0;
					for (int j = 0; j < n; j++)
					{
						if (elements[i][j] && elements[i][j]->is_active())
						{
							ws[i][j] = x(size_matr);
							size_matr++;
						}
					}
				}
			}

			for (int j = 0; j < n; j++)  // Не исправлено, просто копия того, что выще
			{
				size_matr = 0;
				for (int i = 0; i < m; i++)
					if (elements[i][j] && elements[i][j]->is_active())
						size_matr++;
				Eigen::SparseMatrix<double> sm(size_matr, size_matr);
				Eigen::VectorXd res(size_matr), x(size_matr);
				size_matr = 0;
				for (int i = 0; i < m; i++)
				{
					if (elements[i][j] && elements[i][j]->is_active())
					{
						mu = 1;
						*difw = 0;
						*difr = 0;
						EdgeElement* ey = elements[i][j]->edge_y;
						if (ey)
						{
							ey->get_edge_dif(difw, difr);
							mu = ey->get_mu();
						}
						if (!ey || (ey && ey->y < elements[i][j]->y))
						{
							sm.insert(size_matr, size_matr + 1) = 2 * h_t / (h * h * mu * (1 + mu));
						}
						if (!ey || (ey && ey->y >= elements[i][j]->y))
						{
							sm.insert(size_matr, size_matr - 1) = 2 * h_t / (h * h * (1 + mu));
						}
						sm.insert(size_matr, size_matr) = -(2 * h_t / (mu * h * h) + 1) + *difw;
						res(size_matr) = -ws[i][j] + *difr;  //Проверить
						size_matr++;
					}
				}
				if (size_matr > 0)
				{
					Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
					solver.compute(sm);
					x = solver.solve(res);
					size_matr = 0;
					for (int i = 0; i < m; i++)
					{
						if (elements[i][j] && elements[i][j]->is_active())
						{
							elements[i][j]->T[t] = x(size_matr);
							size_matr++;
						}
					}
				}
			}
			for (int i = 0; i < edges.size(); i++)
				edges[i]->calculate(t);
		}
	}

	void show_bin()
	{
		cout << "show_bin()\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				cout << bin_mask[i][j] << ' ';
			cout << '\n';
		}
	}
	void show_form()
	{
		cout << "show_form()\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				if (elements[i][j] && (elements[i][j])->get_parent())
				{
					cout << (elements[i][j]->get_parent())->get_form() << ' ';
				}
				else
					cout << "- ";
			cout << '\n';
		}
	}
	void show_active()
	{
		cout << "show_active()\n";
		for (int i = m - 1; i > -1; i--)
		{
			if (i < 10)
				cout << i << "  ";
			else
				cout << i << " ";
			for (int j = 0; j < n; j++)
				if (elements[i][j])
				{
					cout << elements[i][j]->is_active() << ' ';
				}
				else
					cout << "- ";
			cout << '\n';
		}
		cout << "   ";
		for (int j = 0; j < n; j++)
			if (j < 10)
				cout << j << " ";
			else
				cout << j;
		cout << '\n';
	}
	void show_edges()
	{
		cout << "show_edges()\n";
		cout << edges.size() << "!" << endl;
		for (int i = 0; i < edges.size(); i++)
			cout << edges[i]->get_mu() << " ";
		cout << endl;
	}
	void show_T(int t)
	{
		cout << "show_T(" << t << ")\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				if (elements[i][j] && elements[i][j]->is_active())
				{
					if (elements[i][j]->get_T(t) > 9)
						printf("%.0f ", elements[i][j]->get_T(t));
					else
						printf("0%.0f ", elements[i][j]->get_T(t));
				}
				else
					printf("-- ");
			cout << endl;
		}
	}
	void show_edge_cond()
	{
		cout << "show_edge_cond()\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				if (elements[i][j] && elements[i][j]->edge_x && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_x)->cond).type << ((elements[i][j]->edge_y)->cond).type << " ";
				else if (elements[i][j] && elements[i][j]->edge_x)
					cout << ((elements[i][j]->edge_x)->cond).type << "  ";
				else if (elements[i][j] && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_y)->cond).type << "  ";
				else if (elements[i][j])
					cout << "00 ";
				else
					cout << "-- ";
			cout << endl;
		}
	}
	void show_edge_T(int t)
	{
		cout << "show_edge_T(" << t << ")\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				if (elements[i][j] && elements[i][j]->edge_x && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_x)->get_T(t)) << ((elements[i][j]->edge_y)->get_T(t)) << " ";
				else if (elements[i][j] && elements[i][j]->edge_x)
					cout << ((elements[i][j]->edge_x)->get_T(t)) << "  ";
				else if (elements[i][j] && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_y)->get_T(t)) << "  ";
				else if (elements[i][j])
					cout << "00 ";
				else
					cout << "-- ";
			cout << endl;
		}
	}
	void show_edge_mu()
	{
		cout << "show_edge_mu()\n";
		for (int i = m - 1; i > -1; i--)
		{
			for (int j = 0; j < n; j++)
				if (elements[i][j] && elements[i][j]->edge_x && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_x)->get_mu()) << ((elements[i][j]->edge_y)->get_mu()) << " ";
				else if (elements[i][j] && elements[i][j]->edge_x)
					cout << ((elements[i][j]->edge_x)->get_mu()) << "  ";
				else if (elements[i][j] && elements[i][j]->edge_y)
					cout << ((elements[i][j]->edge_y)->get_mu()) << "  ";
				else if (elements[i][j])
					cout << "00 ";
				else
					cout << "-- ";
			cout << endl;
		}
	}
};

void m_test()
{
	Eigen::SparseMatrix<double> sm(3, 3);
	Eigen::VectorXd res(3), x(3);

	for (int j = 0; j < 3; j++)
	{
		sm.insert(j, j) = (j + 1) * 2;
		res(j) = (j + 1) * 2;
	}
	Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
	solver.compute(sm);
	x = solver.solve(res);

	cout << x(0) << " " << x(1) << " " << x(2) << endl;

	int a;
	cin >> a;
}

int main()
{
	//m_test();

	int h = 10;

	Object obj(500, 400, h, 200, 1);

	Rectangle r1(0, 0, 350, 400, EdgeCondition(3, 100)), r2(340, 0, 500, 250, EdgeCondition(3, 100));
	Circle c1(350, 250, 150, EdgeCondition(3, 200));

	Rectangle rin(105, 205, 205, 305, EdgeCondition(5));
	Rectangle rr(500, 0, 500, 250, EdgeCondition(5)), rl(0, 0, 0, 400, EdgeCondition(5));
	Rectangle rt(0, 400, 350, 400, EdgeCondition(3, 100)), rd(0, 0, 500, 0, EdgeCondition(3, 100));

	Rectangle test0(0, 0, 40, 50.1, EdgeCondition(3, 100));
	Rectangle test1(0, 0, 0, 50, EdgeCondition(5));
	Rectangle test2(40, 0, 40, 50, EdgeCondition(5));
	
	/*
	Object obj(40 + h, 50 + h, h, 100, 1);

	obj.add_form(&test0, 0, 1);
	obj.add_form(&test1, 0, 0);
	obj.add_form(&test2, 0, 0);
	//*/

	obj.add_form(&r1, 0, 1);
	obj.add_form(&r2, 0, 1);
	obj.add_form(&c1, 0, 1);
	
	obj.add_form(&rin, 0, 0);
	obj.add_form(&rr, 0, 0);
	obj.add_form(&rl, 0, 0);
	obj.add_form(&rt, 0, 0);
	obj.add_form(&rd, 0, 0);
	//*/

	//cout << c1.get_dist_x(360, 390);

	obj.show_bin();
	obj.show_active();
	//obj.show_form();

	obj.compile_edges();

	obj.show_bin();
	obj.show_active();
	
	obj.show_edges();
	obj.show_edge_cond();
	obj.show_edge_mu();

	//obj.show_active();
	obj.calculate_implicit();
	obj.show_edge_T(100);
	//obj.show_T(1);
	obj.show_T(100);

	obj.calculate_explicit();
	obj.show_edge_T(100);
	//obj.show_T(1);
	obj.show_T(100);
}