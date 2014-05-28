/**inheritanceMain.cpp*/

#include "square.h"
#include "triangle.h"
#include "circle.h"

using namespace std;

int main()
{
	//declares three pointers to objects of the class shape
	shape *s1; //s1 is a pointer to a shape type
	square *s2; //s2 is a pointer to a square type
	triangle *s3; //s3 is a pointer to a triangle type
	circle *s4; //s4 is a pointer to a circle type
	
	//prints a header
	cout << endl << "WELCOME TO C++ INHERITANCE ****************" << endl << endl;

	//allocates memory calling constructors
	s1 = new square("square1", 2); //s1 will be a square of side 2
	s2 = new square("square2", 3); //s2 will be a square of side 3	
	s3 = new triangle("triangle1", 2, 4); //s3 wilb a triangle 2x4
	s4 = new circle("circleA",2);
	
	//do something
	cout << "Area of "; s1->printName(); cout << " is: " << s1->computeArea() << endl;
	cout << "Area of "; s2->printName(); cout << " is: " << s2->computeArea() << endl;
	cout << "Area of "; s3->printName(); cout << " is: " << s3->computeArea() << endl;
	cout << "Area of "; s4->printName(); cout << " is: " << s4->computeArea() << endl;
	
	//draw shapes
	s1->draw();
	s3->draw();
	s4->draw();
	cout << "********************************************" << endl;
		
	//calls destructor just for s1
	delete s1;
	
	//since s1 was a "generic" pointer to "shape" type, it can be used to construct another kind of shape
	s1 = new triangle("triangle2",3,3);
	cout << "Area of "; s1->printName(); cout << " is: " << s1->computeArea() << endl;

	//Finally call all destructors -> free reserved memory space 
	delete s1;
	delete s2;
	delete s3;
	delete s4;

	return 0;
}
