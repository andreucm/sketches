
#include "dlib/matrix.h"
#include <iostream>
// #include <vector>

using namespace std;
using namespace dlib;

int main()
{
	matrix<double> v1;
      matrix<double,4,4> m1,m2;
      matrix<double,2,2> m3;
      matrix<double,2,1> v2;
	
	v1.set_size(1,3);
	v1 = 1.1,2.2,3.3;
	cout << "v1.size(): " << v1.size() << endl;
	cout << "v1: " << trans(v1) << endl;
	
	v1.set_size(6,1);
	//set_subm(v1,range(3,4)) = v2;
	set_rowm(v1,range(3,5)) = 4.4,
				  5.5,
				  6.6;
	cout << "v1.size(): " << v1.size() << endl;
      cout << "v1: " << trans(v1) << endl;
	
      m1 = dlib::identity_matrix(dlib::matrix<double>(4,4));
      m1(0,1) = -1;
      cout << subm(m1,range(0,1),range(0,1));
      cout << "v1(0,1): " << trans(subm(v1,range(0,1),range(0,0)));
      
      v2 = -1.1,-2.2;
      dlib::set_colm( v1, range(0,1) ) = v2;
      cout << "v1(0,1): " << trans(subm(v1,range(0,1),range(0,0)));
      
      m2 = 5*dlib::ones_matrix(dlib::matrix<double>(4,4));
      m1=m2;
      cout << m1 << std::endl;
      
      matrix<double,10,1> va;
      matrix<double,3,1> vb, vc, vd;
      va = zeros_matrix(dlib::matrix<double>(10,1));
      vb = 1,2,3;
      vc = 4,5,6;        
      set_colm(va,range(0,2)) = vb;
      set_colm(va,range(3,5)) = vc;
      cout << "va: " << trans(va) << endl;
      vc += vb*2;
      cout << "vc: " << trans(vc) << endl;
      //cout << "vc modulus: " << vc.mod() << endl;
      //vc = vc/10;
      vc = vc - 10;
      cout << "vc: " << trans(vc) << endl;
      
      //set a 3-vector as a part of a 10-vector
      vd = subm(va,range(0,2),range(0,0));
      cout << "vd: " << trans(vd) << endl;
      
//         cout << std::vector<double>(1,2).at(0) << endl;
//         cout << std::vector<double>(1,2).at(1) << endl;
      
      m2 = dlib::identity_matrix(dlib::matrix<double>(4,4))*1e-10;
      cout << "m2: " << m2 << endl;
      
      vb = 1,2,3;
      vc = vb/2.;
      cout << "vc: " << trans(vc) << endl;
      
      m1 = 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16;
      cout << "m1: " << endl << m1 << endl;
      
      m3 = 0,1,2,3;
      //set_subm(m1,range(2,3),range(2,3)) = 0.,1.,2.,3.;
      set_subm(m1,range(2,3),range(2,3)) = m3;
      //set_rowm(m1,range(2,3),range(2,3)) = 0,1,2,3;
      cout << "m1: " << endl << m1 << endl;
            
	return 0;
}



