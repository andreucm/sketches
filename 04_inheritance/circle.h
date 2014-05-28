/**circle.h*/

#ifndef circle_h
#define circle_h

#include <math.h>
#include "shape.h" 


using namespace std;

class circle : public shape
{
	protected:
		float radius;
	public:
		circle();
		circle(const string & strName, float sd);
		~circle();
		void setRadius(float sd);
		float computeArea();
		void draw();
};

circle::circle():shape("circle")
{
	radius=1; //sets default radius
	cout << "circle constructor. Name: " << name << "; radius = " << radius << endl;
} 

circle::circle(const string & strName, float sd):shape(strName)
{
	radius=sd; 
	cout << "circle constructor. Name: " << name << "; radius = " << radius << endl;
} 

circle::~circle()
{
	cout << "circle destructor. Name: " << name << "; radius = " << radius << endl;
}

void circle::setRadius(float sd)
{
	radius = sd;
}

float circle::computeArea()
{
	return M_PI*radius*radius;
}

void circle::draw()
{
	cout << endl  << "   o" << endl << endl;
}

#endif
