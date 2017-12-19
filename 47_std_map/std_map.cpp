
#include <iostream>
#include <map>


class myClass
{
	protected:
		double a_;
		int b_;

	public:
        myClass()
        {
              a_ = 0.1;
			  b_ = 1;
        }

		myClass(double _a, int _b)
		{
			a_ = _a;
			b_ = _b;
			std::cout << "End of constructor" << std::endl;
			std::cout << "\ta_ = " << a_ << std::endl;
			std::cout << "\tb_ = " << b_ << std::endl;
		}

		~myClass()
		{
			std::cout << "End of destructor" << std::endl;
		}

		void setA(double _a)
		{
			a_ = _a;
		}

		void setB(double _b)
		{
			b_ = _b;
		}

		double getA() const
		{
			return a_;
		}

		int getB() const
		{
			return b_;
		}

		double product() const
		{
			return (a_*b_);
		}
};

int main()
{
	std::map<std::string, myClass> map;

	map["pose1"].setA(1.1);
	map["pose1"].setB(11);

	map["pose2"].setA(2.2);
	map["pose2"].setB(22);

	map["pose3"].setA(3.3);
	map["pose3"].setB(33);

	std::cout << "map[\"pose3\"].product(): " << map["pose3"].product() << std::endl;
	std::cout << "map[\"pose2\"].getA(): " << map["pose2"].getA() << std::endl;


	return 0;
}
