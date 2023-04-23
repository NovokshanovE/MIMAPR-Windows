#include "Object.h"
#include "Point.h"

int main()
{
	int variant;
	std::cin >> variant;
	int p1, p2;
	cin >> p1 >> p2;
 	Object* body = new Object(variant, p1, p2);
	body->print();
	//ArrayXXf
	int in_put;
	cin >> in_put;
}