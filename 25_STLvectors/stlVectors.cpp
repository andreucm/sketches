
#include <iostream>
#include <vector>

using namespace std;

class myClass
{
	protected: 
		double aa;
	public:
            myClass()
            {
                  aa=0;
            }
                
		myClass(double value)
		{
			aa = value;
			cout << "End of constructor: aa=" << aa << endl;
		}
		
		~myClass()
		{
			cout << "End of destructor: aa=" << aa << endl;
		}
		
		void setValue(double value)
		{
			aa=value;
		}
		
		double getValue()
                {
                        return aa;
                }
		
		double by2()
		{
			return (2*aa);
		}
};

int main()
{
	vector<myClass> set;
	myClass * newMember;
	int ii;
	
	for (ii=0; ii<4; ii++)
	{
		newMember = new myClass( (double) (ii+1) );
		//newMember->setValue( (double)ii );
		set.push_back(*newMember);
	}
	
	cout << "Results: ";
	for (ii=0; ii<set.size(); ii++)
	{
		cout << set.at(ii).by2() << "; ";
	}
	cout << endl;
	
	set.clear();	
        
      //*************
      set.resize(5);
      set.at(4).setValue(44);
      cout << "Results: ";
      for (ii=0; ii<set.size(); ii++)
      {
            cout << set.at(ii).getValue() << "; ";
      }
      cout << endl;
        
	return 0;
}
