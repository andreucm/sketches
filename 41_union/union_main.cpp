
/**union_main.cpp*/

#include <iostream>

union long2double {
	double d;
	unsigned long ul;
};

int main()
{
	long2double num_;
	unsigned long * ul_ptr_; 
	double * d_ptr_;
	
	num_.d = -0.1234;
	ul_ptr_ = &num_.ul; 
	d_ptr_ = &num_.d; 
	
	std::cout << "num_.d: " << num_.d << std::endl
			  << "num_.ul: " << std::dec << num_.ul << std::dec << std::endl
			  << "*ul_ptr_: " << *ul_ptr_ << std::endl
			  << "*d_ptr_: " << std::dec << *d_ptr_ << std::dec << std::endl;
	
	return 0;
}
