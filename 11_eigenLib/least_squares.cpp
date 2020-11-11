
//Std
#include <iostream>
#include <deque>

//eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

double computeLS(const std::deque<double> __buffer)
{
	// do not compute the derivative if 1 or 2 points only
	if ( __buffer.size() < 3 ) return 0.0;

	// set the LS problem
	Eigen::MatrixXd D(__buffer.size(), 3);
	Eigen::VectorXd y(__buffer.size());
	for (unsigned int ii=0; ii<__buffer.size(); ii++)
	{
		D(ii,0) = (ii+1)*(ii+1);
		D(ii,1) = ii+1;
		D(ii,2) = 1.0;
		y(ii) = __buffer[ii];
	}

	// compute the pseudo-inverse
	Eigen::MatrixXd D_pinv;
	D_pinv = ( (D.transpose()*D).inverse() )*D.transpose();

	// solve
	Eigen::Vector3d x;
	x = D_pinv*y;
	std::cout << "x: " << x.transpose() << std::endl;
	return x(0);
}

int main(int argc, char *argv[])
{
	std::deque<double> data;

	data.push_back(2);
	data.push_back(4);
	data.push_back(7);
	data.push_back(14);

	double result = computeLS(data);


	return 0;
}
