

//std includes
#include <iostream>
#include <random>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>



//struct definig a cost function as a templated const operator ()
struct MyCostFunctor 
{
    template <typename T> bool operator()(const T* const x, T* residual) const 
    {
        residual[0] = T(10.0) - x[0];
        return true;
    }
};

//main 
int main(int argc, char** argv) 
{

    //program arguments. Num points, center of the sphere and radius
    unsigned int np = 100; 
    Eigen::Vector3d center << 5,-6,7; 
    double rr = 3.7; 
    noise_stddev = 0.2; 
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);

    // Declare the ceres problem.
    ceres::Problem problem;

    //generate spherical points with noise and outliers
    std::default_random_engine rnd_gen;
    std::uniform_real_distribution<double> rnd_uniform(0.0,1.0);
    std::normal_distribution<double> rnd_normal(5.0,2.0);    
    Eigen::MatrixXd points;
    points.resize(3,np);
    Eigen::Quaterniond q_rnd;
    Eigen::Matrix3d m_rnd; 
    Eigen::Vector3d v_rnd; 
    for (unsigned int ii=0; ii<np; ii++)
    {
        q_rnd.vec() << rnd_uniform(rnd_gen), rnd_uniform(rnd_gen), rnd_uniform(rnd_gen); 
        q_rnd.w() = sqrt(1 -  ); 
        points.block<3,1>(0,ii) = center + 
    }
    
    //add noise to points

    //create measurements and add constraints to th problem

    //solve: estimate sphere parameters

    //print results
    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x << "\n";
    
    //exit
    return 0;
}
