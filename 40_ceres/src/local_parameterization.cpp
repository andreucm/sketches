

//std includes
#include <iostream>
#include <random>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

//This sketch wants to find the best orientation of a L-shapped stick that minimizes the distance 
//between the end of te stick and a set of points.
//The L-shapped stick is attached to the origin, with 1m link from the origin to the elbow, and 0.1m link from the elbow to the end.
//Long primary link is along object X-axis, and short secondary link is along object Y-axis
const double STICK_LONG_LINK_SIZE = 1; 
const double STICK_SHORT_LINK_SIZE = 0.1; 

//class holding a point constraint and definig an Auto diff cost function as a templated const operator ()
class pointConstraint
{
    private:
        //point measurement
        Eigen::Vector3d point_sensor; //a single point measured in sensor frame
        unsigned int point_id_; //associates the measured point with its id in the map. Just to avoid data association step. 
        std::vector<Eigen::Vector3d> *map_; // a set of points wrt global frame
    
    public: 
        pointConstraint(const Eigen::Vector3d & _point) :
            point_(_point)
        {
            //
        }
        
        ~pointConstraint()
        {
            //
        }
        
        template <typename T> bool operator()(const T* const _q, T* _residual) const 
        {
            Eigen::Quaternion<T> quat(_q);//constructor from the array data. Storing order in eigen is qi,qj,qk,qr. 
            Eigen::Vector<T,3,3> matR;
            Eigen::Vector<T,3,3> point_map; 
            
            //build the rotation matrix from the quaternion
            matR = quat.toRotationMatrix();
            
            //move the locally measured point to the global frame
            point_map = matR*(*point_sensor); 
            
            //compute the residual (squared distance) between the measured point in the global frame and its correpsonding map point 
            _residual = ( T( (*map_)[point_id_].x() ) - point_map.x() )^2 + 
                        ( T( (*map_)[point_id_].y() ) - point_map.y() )^2 + 
                        ( T( (*map_)[point_id_].z() ) - point_map.z() )^2 ; 

            //return 
            return true;
        }
};

//main 
int main(int argc, char** argv) 
{

    //user arguments. 
    unsigned int np = 500; //num of map points
    double map_side = 10; //the map is like a cube of map_side side
    double noise_stddev = 0.1; //noise added to measured points
    double outlier_ratio = 0.05; //outlier ratio
    Eigen::Quaterniond true_orientation(1,0,0,0); 
    Eigen::Quaterniond initial_orientation(1,0,0,0); 
    bool use_loss_function = true;     
    
    //params to be optimized
    Eigen::Quaterniond optimized_orientation(initial_orientation); 
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);


//************** GERNERATE MEASUREMENTS *************************
    
    std::default_random_engine rnd_gen;
    std::uniform_real_distribution<double> rnd_uniform(-0.5*map_side,0.5*map_side);
    std::normal_distribution<double> rnd_normal(0.,noise_stddev);    
    Eigen::MatrixXd point_map(3,np);
    Eigen::Vector3d noise; 
    
    //generate map points within the cube 
    for (unsigned int ii=0; ii<np; ii++)
    {
        point_map.block<3,1>(0,ii)  << rnd_uniform(rnd_gen), rnd_uniform(rnd_gen), rnd_uniform(rnd_gen);
    }
    
    //generate local measurements and add normal noise to them
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
