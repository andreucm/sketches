/**triangle.h*/

#ifndef triangle_h
#define triangle_h

#include "shape.h" 

using namespace std;

class triangle : public shape
{
	protected:
		float base;
		float height;
	public:
		triangle();
		triangle(const string & strName, float bb, float hh);
		~triangle();
		void setBase(float bb);
		void setHeight(float hh);
		float computeArea();
		void draw();
};

triangle::triangle():shape("triangle")
{
	base=1; //sets default side
	height=1;
	cout << "triangle constructor. Name: " << name << "; base = " << base << "; height = " << height << endl;
} 

triangle::triangle(const string & strName, float bb, float hh):shape(strName)
{
	base=bb;
	height=hh;
	cout << "triangle constructor. Name: " << name << "; base = " << base << "; height = " << height << endl;
} 

triangle::~triangle()
{
	cout << "triangle destructor. Name: " << name << "; base = " << base << "; height = " << height << endl;
}

void triangle::setBase(float bb)
{
	base = bb;
}

void triangle::setHeight(float hh)
{
	height = hh;
}

float triangle::computeArea()
{
	return base*height/2;
}

void triangle::draw()
{
	cout << "   ." << endl;
	cout << "  / \\" << endl;
	cout << " /   \\" << endl;
	cout << "-------" << endl;
}

#endif
