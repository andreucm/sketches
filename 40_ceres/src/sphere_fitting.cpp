

//std includes
#include <iostream>
#include <random>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>


enum diffType {AUTOMATIC=1, NUMERIC};

//class holding a point measurement and definig an Auto diff cost function as a templated const operator ()
class SphereConstraintAD
{
    private:
        //point measurement
        Eigen::Vector3d point_; 
    
    public: 
        SphereConstraintAD(const Eigen::Vector3d & _point) :
            point_(_point)
        {
            //
        }
        
        ~SphereConstraintAD()
        {
            //
        }
        
        template <typename T> bool operator()(const T* const _xc, const T* const _xr, T* _residual) const 
        {
            //sphere equation (x-cx)^2+(y-cy)^2+(z-cz)^2 = R^2
            _residual[0] = (T(point_.x())-_xc[0])*(T(point_.x())-_xc[0]) +
                           (T(point_.y())-_xc[1])*(T(point_.y())-_xc[1]) +
                           (T(point_.z())-_xc[2])*(T(point_.z())-_xc[2]) -
                           _xr[0]*_xr[0];
            return true;
        }
};

//class holding a point measurement and definig a Numeric diff cost function as a templated const operator ()
class SphereConstraintND
{
    private:
        //point measurement
        Eigen::Vector3d point_; 
    
    public: 
        SphereConstraintND(const Eigen::Vector3d & _point) :
            point_(_point)
        {
            //
        }
        
        ~SphereConstraintND()
        {
            //
        }
        
        bool operator()(const double* const _xc, const double* const _xr, double* _residual) const 
        {
            //sphere equation (x-cx)^2+(y-cy)^2+(z-cz)^2 = R^2
            _residual[0] = (point_.x()-_xc[0])*(point_.x()-_xc[0]) +
                           (point_.y()-_xc[1])*(point_.y()-_xc[1]) +
                           (point_.z()-_xc[2])*(point_.z()-_xc[2]) -
                           _xr[0]*_xr[0];
            return true;
        }
};


//main 
int main(int argc, char** argv) 
{

    //user arguments. Num points, center of the sphere and radius
    unsigned int np = 500; 
    Eigen::Vector3d center; 
    center  << 5,-6,7; 
    double radius = 3.7; 
    double noise_stddev = 0.1; 
    double outlier_ratio = 0.05; 
    Eigen::Vector3d initial_center; 
    initial_center << 0,0,0; //initial guess for the center
    double initial_radius = 1; //initial guess for the radius
    bool use_loss_function = true; 
    diffType diff_type = AUTOMATIC; //AUTOMATIC or NUMERIC
    
    
    //params to be optimized
    Eigen::Vector3d x_opt_center(initial_center);
    double x_opt_radius(initial_radius); 
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);


//************** GERNERATE MEASUREMENTS *************************
    
    //generate points on the sphere surface
    std::default_random_engine rnd_gen;
    std::uniform_real_distribution<double> rnd_uniform(0.0,1.0);
    std::normal_distribution<double> rnd_normal(0.,noise_stddev);    
    Eigen::MatrixXd points;
    points.resize(3,np);
    Eigen::Vector3d v_rnd;
    Eigen::Vector3d noise; 
    for (unsigned int ii=0; ii<np; ii++)
    {
        v_rnd << rnd_uniform(rnd_gen)-0.5, rnd_uniform(rnd_gen)-0.5, rnd_uniform(rnd_gen)-0.5;
        v_rnd.normalize(); 
        points.block<3,1>(0,ii) = center + v_rnd*radius;
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
        if ( rnd_uniform(rnd_gen) < outlier_ratio ) //outlier case
        {
            points.block<3,1>(0,ii) << points.block<3,1>(0,ii)*0.5*rnd_uniform(rnd_gen); 
        }
    }
    
//***************************************************************

//*************** CERES PROBLEM *********************************

    // Declare the ceres problem.
    ceres::Problem problem;

    switch (diff_type)
    {
        //Add constraints to the problem, according user params: use_loss_function and diff_type
        case AUTOMATIC:
            if ( use_loss_function )
            {
                for (unsigned int ii=0; ii<np; ii++)
                {
                    problem.AddResidualBlock(
                        new ceres::AutoDiffCostFunction<SphereConstraintAD,1,3,1>(new SphereConstraintAD(points.block<3,1>(0,ii))),
                        new ceres::CauchyLoss(0.5), 
                        x_opt_center.data(), &x_opt_radius );    
                }
            }
            else
            {
                for (unsigned int ii=0; ii<np; ii++)
                {
                    problem.AddResidualBlock( 
                        new ceres::AutoDiffCostFunction<SphereConstraintAD,1,3,1>(new SphereConstraintAD(points.block<3,1>(0,ii))),
                        NULL, 
                        x_opt_center.data(), &x_opt_radius );    
                }   
            }
            break; 
            
        case NUMERIC:
            for (unsigned int ii=0; ii<np; ii++)
            {
                problem.AddResidualBlock( 
                    new ceres::NumericDiffCostFunction<SphereConstraintND,ceres::CENTRAL,1,3,1>(new SphereConstraintND(points.block<3,1>(0,ii))),
                    new ceres::CauchyLoss(0.5), 
                    x_opt_center.data(), &x_opt_radius );    
            }   
            break;
            
        default: break;
    }
    
    //set bounds
    problem.SetParameterLowerBound(&x_opt_radius,0,1e-3); 
    
    //solve: estimate sphere parameters
    ceres::Solver::Options options;
    options.minimizer_type = ceres::TRUST_REGION;
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);    

    //print results
    std::cout << summary.BriefReport() << std::endl;
    std::cout << "x_opt_center : " << x_opt_center.transpose() << std::endl;
    std::cout << "x_opt_radius : " << x_opt_radius << std::endl;
    
    //exit
    return 0;
}
