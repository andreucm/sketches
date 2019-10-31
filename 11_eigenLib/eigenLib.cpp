/**helloWorld.cpp*/

//Std
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <cstdlib>

//eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

// import most common Eigen types
using namespace std;
using namespace Eigen;

int main(int argc, char *argv[])
{

	Matrix3f m3, m3i;
	Matrix3f *mp3;
	Matrix4f m4 = Matrix4f::Identity();
	Vector4i v4(1, 2, 3, 4);
	Vector3f uu,vv,ww;
	Matrix<double, 6, 6> mz = Matrix<double, 6, 6>::Zero(6,6);
	double aa;
    timeval ts;
    double t1, t2;
    MatrixXi bigMatrix, bigMatrixResult;
    int mSize = 1500;
    MatrixXd md;

    //get user input
    int user_choice = atoi(argv[1]);

    //main switch
    switch(user_choice)
    {
        case 1:
            std::cout << std::endl << "Test1: set and access" << std::endl;
            cout << mz << endl;
            m3 << 1, 2, -3, 4, -5, 6, -7, 8, 9;
            cout << "m3\n" << m3 << "\nm4:\n" << m4 << "\nv4:\n" << v4 << endl;
            cout << "m3(1,2) = " << m3(1,2) << endl;
            cout << "v4(0) = " << v4(0) << " = " << v4[0] << " = " << v4.x() << endl;
            cout << "v4(3) = " << v4(3) << " = " << v4[3] << " = " << v4.w() << endl;
            cout << "m3(1,:) = " << m3.block<1,3>(1,0) << " = "<< m3.block(1,0,1,3) << endl;
            cout << "m3(:,1) = \n" << m3.block<3,1>(0,1) << endl;
            break;

        case 2:
            std::cout << std::endl << "Test2: operations" << std::endl;
            m3 << 1, 2, -3, 4, -5, 6, -7, 8, 9;
            m3i=m3.inverse();
            cout << "\nm3i:\n" << m3i <<endl;
            cout << "(m3*m3i).inverse() = " << (m3*m3i).inverse() << endl;
            uu << 1, 0, -4.2;
            vv << -2.5, 1, 2.1;
            ww = uu.cross(vv);//cross product
            cout << "ww = uu x vv =\n" << ww << endl;
            aa = uu.dot(vv);//dot product
            cout << "aa = uu · vv = " << aa << endl;
            m3 = uu*vv.transpose();//outer product
            cout << "m3 = uu · vv' = \n" << m3 << endl;
            ww = m3*uu;//matrix by vector
            cout << "ww = m3 x uu =\n" << ww << endl;
            cout << "(uu-vv).transpose() = " << (uu-vv).transpose() << endl;
            mp3 = new Matrix3f();
            *mp3 << 11, 22, -33, 44, -55, 66, -77, 88, 99;
            cout << "(*mp3)(0,0)=" << (*mp3)(0,0) << endl;
            uu = vv+3*Vector3f::Ones();
            break;

        case 3:
            std::cout << std::endl << "Test3: matrixXf" << std::endl;

            gettimeofday(&ts, NULL);
            t1 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            bigMatrix.resize(mSize,mSize);
            gettimeofday(&ts, NULL);
            t2 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            cout << "Time resizing: " << t2-t1 << endl;

            int ii,jj;
            gettimeofday(&ts, NULL);
            t1 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            for (ii=0; ii<bigMatrix.rows(); ii++)
            {
                for (jj=0; jj<bigMatrix.cols(); jj++)
                {
                    bigMatrix(ii,jj) = (ii+jj);
                }
            }
            gettimeofday(&ts, NULL);
            t2 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            cout << "Time filling the matrix: " << t2-t1 << endl;

            bigMatrixResult.resize(mSize, mSize);
            gettimeofday(&ts, NULL);
            t1 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            bigMatrixResult = bigMatrix*bigMatrix;
            gettimeofday(&ts, NULL);
            t2 = (double)(ts.tv_sec + ts.tv_usec/1e6);
            cout << "Time for matrix product: " << t2-t1 << endl;
            break;

        case 4:
            md.setZero(1,3);
            cout << md << endl;
            break;

        case 5:
            uu << 1, 0, -4.2;
            std::cout << uu.block<2,1>(1,0) << std::endl;
			break;

		case 6:
		{
			Matrix3d AA, BB;
			AA = Eigen::Matrix3d::Identity();
			BB = Eigen::Matrix3d::Zero();
			MatrixXd CC(3,6);
			CC << AA,BB;
			std::cout << "CC: " << std::endl << CC << std::endl;
		}
			break;

        default:
            std::cout << std::endl << "Unknown Test Id. EXIT" << std::endl;
            break;
    }

    return 0;
}
//results:
//mSize	Product time [s]
//500		4.5
//1000	38
//1500	122
//2000	290
//3000	986
