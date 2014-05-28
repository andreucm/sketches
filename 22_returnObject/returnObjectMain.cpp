#include <iostream>
#include <list>

using namespace std;

class wheel
{
	protected:
		double radius;
		double width;
	public:
		wheel(){}
		wheel(double rr, double ww)
		{
			radius = rr;
			width = ww;
		}
		~wheel(){}
		double getR(){return radius;}
		double getW(){return width;}
		void printWheel(){cout << "r = " << radius << "; w = " << width << endl;}
};

class vehicle
{
	protected:
		list<wheel> wheels;
	public:
		vehicle(){}
		~vehicle(){wheels.clear();}
		void addWheel(double radius , double width)
		{
			wheel * newWheel = new wheel(radius,width);
			wheels.push_back(*newWheel);
		}
		void addWheel(wheel & wl)
		{
			wheels.push_back(wl);
		}
		wheel * getWheelPtr(unsigned int ii)
		{
			unsigned int jj=0;
			list<wheel>::iterator iiW;
			iiW = wheels.begin();
			while(jj!=ii)
			{
				jj++;
				iiW++;
			}
			return &(*iiW);
		}
		wheel getWheelObj(unsigned int ii)
		{
			unsigned int jj=0;
			list<wheel>::iterator iiW;
			iiW = wheels.begin();
			while(jj!=ii)
			{
				jj++;
				iiW++;
			}
			return (*iiW);		
		}
		wheel & getWheelRef(unsigned int ii)
		{
			unsigned int jj=0;
			list<wheel>::iterator iiW;
			iiW = wheels.begin();
			while(jj!=ii)
			{
				jj++;
				iiW++;
			}
			return (wheel &)(*iiW);
		}		
		void getWheelParam(unsigned int ii, wheel & ww)
		{
			unsigned int jj=0;
			list<wheel>::iterator iiW;
			iiW = wheels.begin();
			while(jj!=ii)
			{
				jj++;
				iiW++;
			}
			ww = (*iiW);
		}
		void printVehicle()
		{
			list<wheel>::iterator iiW;
			cout << "------------------------" << endl;
			for(iiW=wheels.begin(); iiW!=wheels.end(); iiW++){iiW->printWheel();}
			cout << "------------------------" << endl;
		}
};

int main()
{
	wheel * myWheelPtr;
	wheel myWheelObj;
	vehicle *myEngine;
	
	//allocate memory for object 
	myEngine = new vehicle();
	
	//build my strange tricycle
	myEngine->addWheel(1,0.05);
	myEngine->addWheel(2,0.1);
	myEngine->addWheel(3,0.2);
	myEngine->printVehicle();
	
	//gets wheel with pointer
	myWheelPtr = myEngine->getWheelPtr(2);
	myWheelPtr->printWheel();
	
	//gets wheel with object
	myWheelObj = myEngine->getWheelObj(2);
	myWheelObj.printWheel();
	
	//gets wheel with reference
	wheel & myWheelRef = myEngine->getWheelRef(1);
	myWheelRef = myEngine->getWheelRef(2);
	myWheelRef.printWheel();
	
	myEngine->getWheelParam(1,myWheelObj);
	myWheelObj.printWheel();
	
	//liberate memory
	delete myEngine;
	
	//Check if myWheel's keep turning
	cout << "-------------------" << endl;
	myWheelObj.printWheel();
	myWheelRef.printWheel(); //this could cause a seg fault since myEngine has been deleted
	myWheelPtr->printWheel(); //this could cause a seg fault since myEngine has been deleted
	
	//main return value 
	return 0;
}
