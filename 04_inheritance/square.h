/**square.h*/

#ifndef square_h
#define square_h

#include "shape.h" 

using namespace std;

class square : public shape
{
	protected:
		float side;
	public:
		square();
		square(const string & strName, float sd);
		~square();
		void setSide(float sd);
		float computeArea();
		void draw();
};

square::square():shape("square")
{
	side=1; //sets default side
	cout << "square constructor. Name: " << name << "; side: " << side << endl;
} 

square::square(const string & strName, float sd):shape(strName)
{
	side=sd; 
	cout << "square constructor. Name: " << name << "; side: " << side << endl;
} 

square::~square()
{
	cout << "square destructor. Name: " << name << "; side: " << side << endl;
}

void square::setSide(float sd)
{
	side = sd;
}

float square::computeArea()
{
	return side*side;
}

void square::draw()
{
	cout << "-------" << endl;
	cout << "|     |" << endl;
	cout << "|     |" << endl;
	cout << "-------" << endl;
}

#endif
