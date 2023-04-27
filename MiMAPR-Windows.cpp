#include "Object.h"
#include "Point.h"

int main()
{
	int variant;
	int step;
	cout << "step:";
	std::cin >> step;
	cout << "variant:";
	std::cin >> variant;
	cout << "p1 and p2:" << endl;
	int p1, p2;
	cin >> p1 >> p2;
	vector<int> boundary;
	for (int i = 0; i < 6; i++) {
		cout << i+1 << ')';
		int a;
		cin >> a;
		boundary.push_back(a);
		
	}
 	Object* body = new Object(variant, p1, p2, step, boundary);
	
	//body->SetStep(step);
	body->PrintTempGrid(0);
	for (int i = 1; i < 100; i++) {
		body->NextStep(false);
		if (i % 10 == 0) {
			body->PrintTempGrid(i);
			body->PrintGrid(i);
		}
		
	}

	//ArrayXXf
	//int in_put;
	//cin >> in_put;
}