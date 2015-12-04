
#include <iostream>
#include "ceres/ceres.h"
#include "glog/logging.h"

int main(int argc, char** argv)
{

    google::InitGoogleLogging(argv[0]);
	
	ceres::Jet<double, 3> jet1(0); 
	ceres::Jet<double, 3> jet2(0); 
	
	jet2 = sqrt(jet1);
	std::cout << "jet2.a: " << jet2.a  << std::endl;

    return 0;

}
