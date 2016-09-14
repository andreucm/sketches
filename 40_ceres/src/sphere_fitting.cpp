

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
    double radius = 3.7; 
    double noise_stddev = 0.2; 
    double outlier_ratio = 0.1; 
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);

    // Declare the ceres problem.
    ceres::Problem problem;

//************** GERNERATE MEASUREMENTS *************************
    
    //generate points on the sphere surface
    std::default_random_engine rnd_gen;
    std::uniform_real_distribution<double> rnd_uniform(0.0,1.0);
    std::normal_distribution<double> rnd_normal(0.,noise_stddev);    
    Eigen::MatrixXd points;
    points.resize(3,np);
    Eigen::Quaterniond q_rnd;
    Eigen::Matrix3d m_rnd; 
    Eigen::Vector3d v_rnd;
    Eigen::Vector3d noise; 
    for (unsigned int ii=0; ii<np; ii++)
    {
        q_rnd.vec() << rnd_uniform(rnd_gen), rnd_uniform(rnd_gen), rnd_uniform(rnd_gen); 
        q_rnd.w() = sqrt( 1 - q_rnd.x()*q_rnd.x() + q_rnd.y()*q_rnd.y() + q_rnd.z()*q_rnd.z() ); 
        m_rnd = q_rnd.toRotationMatrix(); 
        v_rnd = m_rnd.block<3,1>(0,0); //first column is the X vector of the frame
        points.block<3,1>(0,ii) = center + radius*v_rnd; 
    }
    
    //add normal noise to points
    for (unsigned int ii=0; ii<np; ii++)
    {
        noise << rnd_normal(rnd_gen), rnd_normal(rnd_gen), rnd_normal(rnd_gen); 
        points.block<3,1>(0,ii) = points.block<3,1>(0,ii) + noise; 
    }

    //create outliers
    for (unsigned int ii=0; ii<np; ii++)
    {
        if ( rnd_uniform(rnd_gen) < outlier_ratio )
        {
            //
        }
    }
    
    
//***************************************************************

//*************** CERES PROBLEM *********************************

    //create measurements and add constraints to the problem

    //solve: estimate sphere parameters

    //print results
    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x << "\n";
    
    //exit
    return 0;
}
