/**boostEx1.cpp*/

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

int main ()
{
	using namespace boost::numeric::ublas;
	vector<double> vv(3), ww(3);
	matrix<double> AA(3,3), BB(3,3), CC(3,3);
	int ii,jj;
	double t1, t2;

	for (ii = 0; ii < vv.size (); ii++)
	{
		vv(ii) = ii;
		ww(ii) = ii+1;
	}

	std::cout << vv << std::endl;
	std::cout << ww << std::endl;
	
	t1 = inner_prod(vv, ww);
	std::cout << "v*w = " << t1 << std::endl;

	t1 = norm_1(vv); 
	t2 = norm_2(ww); 
	std::cout << "norm1(v) = " << t1 << "; norm2(w) = " << t2 << std::endl;

	AA = outer_prod(vv, ww);
	std::cout << "A = v x w = " << AA << std::endl;

	for (ii = 0; ii < BB.size1(); ii++){
		for (jj = 0; jj < BB.size2(); jj++){
			BB(ii,jj) = ii+jj;}
	}
	std::cout << "B = " << BB << std::endl;

	CC=AA+BB;
	std::cout << "C = A + B = " << CC << std::endl;
}

