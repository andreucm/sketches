
#include <iostream>
#include <vector>
#include <algorithm>

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

      //**************
      double da[] = {1.1,-2.2,3.3,-4.4};
      vector<double> dv(da,da+4);
      cout << "dv = ";
      for (ii=0; ii<dv.size(); ii++) cout << dv[ii] << ", ";
	  cout << std::endl << "dv = ";
	  for (ii=0; ii<dv.size(); ii++) cout << *(dv.data()+ii) << ", ";
	  cout << std::endl << "dv@ = ";
	  for (ii=0; ii<dv.size(); ii++) cout << dv.data()+ii << ", ";
      cout << endl;

	  /*
	  DOES NOT WORK WITH vector<bool> because it has a specialized version of vector. See http://www.cplusplus.com/reference/vector/vector-bool/
	  vector<bool> db(5);
      cout << "db = ";
	  for (ii=0; ii<db.size(); ii++) cout << (int)*(db.data()+ii) << ", ";
	  cout << endl;
	  */

	  std::vector<double> dv2;
	  dv2.push_back(1.1);
	  dv2.push_back(-2.2);
	  dv2.push_back(3.3);
	  dv2.push_back(-4.4);
	  dv2.push_back(-4.4);
	  dv2.push_back(5.5);
	  std::vector<double>::iterator i_dv2;
	  i_dv2 = std::min_element(dv2.begin(), dv2.end());
	  std::cout << "std::min_element(): " << *i_dv2 << std::endl;
	  i_dv2 = std::max_element(dv2.begin(), dv2.end());
	  std::cout << "std::max_element(): " << *i_dv2 << std::endl;

	  return 0;

}
