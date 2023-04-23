#include "Object.h"
#include "Point.h"

int main()
{
	int variant;
	std::cin >> variant;
	int p1, p2;
	cin >> p1 >> p2;
 	Object* body = new Object(variant, p1, p2);
	body->PrintGrid(0);
	//ArrayXXf
	//int in_put;
	//cin >> in_put;
}