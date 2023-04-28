#include "Object.h"
#include "Point.h"
#include <stdio.h>
#include <fstream>
int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	
	
	//fout.close();
	int variant;
	int step;
	//cout << "step:";
	fin >> step;
	//cout << "variant:";
	fin >> variant;
	//cout << "p1 and p2:" << endl;
	int p1, p2;
	fin >> p1 >> p2;
	vector<int> boundary;
	for (int i = 0; i < 6; i++) {
		//cout << i+1 << ')';
		int a;
		fin >> a;
		boundary.push_back(a);
		
	}
 	Object* body = new Object(variant, p1, p2, step, boundary);
	
	//body->SetStep(step);
	fout << "t x y T" << endl;
	fout << '\n';
	//body->PrintTempGrid(0);
	fout.close();
	for (int i = 1; i < 1000; i++) {
		body->NextStep(false);
		
		//if (i % 10000 == 0) {
		//	//body->PrintTempGrid(i);
		//	//body->PrintGrid(i);
		//}
		//body->PrintTempGrid(i);
	}
	
	fin.close();
	body->OutToFile();
	body->PrintTempGrid(999);
	body->PrintGrid(0);
	//ArrayXXf
	//int in_put;
	//cin >> in_put;
}