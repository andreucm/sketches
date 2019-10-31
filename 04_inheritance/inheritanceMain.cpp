/**inheritanceMain.cpp*/

#include "square.h"
#include "triangle.h"
#include "circle.h"

using namespace std;

int main()
{
	//declares three pointers to objects of the class shape
	shape *shape_ptr; //shape_ptr is a pointer to a shape type
	square *square_ptr; //square_ptr is a pointer to a square type
	triangle *triangle_ptr; //triangle_ptr is a pointer to a triangle type
	circle *circle_ptr; //circle_ptr is a pointer to a circle type

	//prints a header
	cout << endl << "WELCOME TO C++ INHERITANCE ****************" << endl << endl;

	//allocates memory calling constructors
	shape_ptr = new square("square1", 2); //shape_ptr will be a square of side 2
	square_ptr = new square("square2", 3); //square_ptr will be a square of side 3
	triangle_ptr = new triangle("triangle1", 2, 4); //triangle_ptr wilb a triangle 2x4
	circle_ptr = new circle("circleA",2);

	//do something
	cout << "Area of "; shape_ptr->printName(); cout << " is: " << shape_ptr->computeArea() << endl;
	cout << "Area of "; square_ptr->printName(); cout << " is: " << square_ptr->computeArea() << endl;
	cout << "Area of "; triangle_ptr->printName(); cout << " is: " << triangle_ptr->computeArea() << endl;
	cout << "Area of "; circle_ptr->printName(); cout << " is: " << circle_ptr->computeArea() << endl;

	//draw shapes
	shape_ptr->draw();
	square_ptr->draw();
	triangle_ptr->draw();
	circle_ptr->draw();

	//calls destructor just for shape_ptr
	delete shape_ptr;

	//since shape_ptr was a "generic" pointer to "shape" type, it can be used to construct another kind of shape
	shape_ptr = new triangle("triangle2",3,3);
	cout << "********************************************" << endl;
	cout << "Area of "; shape_ptr->printName(); cout << " is: " << shape_ptr->computeArea() << endl;
	shape_ptr->draw();

	//Finally call all destructors -> free reserved memory space
	delete shape_ptr;
	delete square_ptr;
	delete triangle_ptr;
	delete circle_ptr;

	return 0;
}
