

//std includes
#include <iostream>
#include <random>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

//This sketch wants to find the best orientation of a sensor that sees several points stored in a map
//points have a unique ID, so data association step is avoided. 
//The purpose is to play & learn around the Local Parameterization API of Ceres library

//class holding a point constraint and definig an Auto diff cost function as a templated const operator ()
class pointConstraint
{
    private:
        Eigen::Vector3d point_sensor_; //a single point measured wrt sensor frame
        unsigned int point_id_; //associates the measured point with its id in the map. Just to avoid data association step. 
        Eigen::MatrixXd *map_; // a set of points wrt global frame. Each column is one point
    
    public: 
        pointConstraint(const Eigen::Vector3d & _point, unsigned int _id, Eigen::MatrixXd * const _map_ptr) :
            point_sensor_(_point),
            point_id_(_id), 
            map_(_map_ptr)
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
            //Eigen::Quaternion<T> quat = Eigen::Map<const Eigen::Quaternion<T> >(_q); //constructor from a map
            Eigen::Matrix<T,3,3> matR;
            Eigen::Matrix<T,3,1> point_map;
            Eigen::Matrix<T,3,1> point_sensor; 
            
            //build the rotation matrix from the quaternion
            matR = quat.toRotationMatrix();
            
            //move the locally measured point to the global frame
            point_sensor << T(point_sensor_.x()),T(point_sensor_.y()),T(point_sensor_.z()); //TODO heck if there is some more efficent way to do this
            point_map = matR*point_sensor; //TODO check if point_map = quat*(*point_sensor); could also work. 
            
            //compute the residual (squared distance) between the measured point in the global frame and its correpsonding map point 
            //3D residual case
            _residual[0] = T( (*map_)(0,point_id_) ) - point_map.x(); 
            _residual[1] = T( (*map_)(1,point_id_) ) - point_map.y();  
            _residual[2] = T( (*map_)(2,point_id_) ) - point_map.z(); 

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
    double outlier_ratio = 0.0; //outlier ratio
    Eigen::Vector3d true_axis(1,2,3);
    true_axis.normalize();
    Eigen::Quaterniond true_orientation( Eigen::AngleAxisd(0.1*M_PI, true_axis) ); //true orientation of sensor wrt map
    bool use_loss_function = true;     
    
    //initial guess
    Eigen::Vector3d iguess_axis(1.1,2.2,2.9);
    iguess_axis.normalize();    
    Eigen::Quaterniond initial_orientation( Eigen::AngleAxisd(0.12*M_PI, iguess_axis) ); //initial guess orientation of sensor wrt map 
    
    //params to be optimized
    Eigen::Quaterniond optimized_orientation(initial_orientation); //optimized orientation of sensor wrt map 
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);


//************** GERNERATE MEASUREMENTS *************************
    
    std::default_random_engine rnd_gen;
    std::uniform_real_distribution<double> rnd_uniform(-0.5*map_side,0.5*map_side);
    std::normal_distribution<double> rnd_normal(0.,noise_stddev);    
    Eigen::MatrixXd map_points(3,np);//points in the map, wrt map frame
    Eigen::MatrixXd measured_points(3,np); //measured points wrt sensor frame
    Eigen::Vector3d noise; 
    
    //generate map points within the cube 
    for (unsigned int ii=0; ii<np; ii++)
    {
        map_points.block<3,1>(0,ii)  << rnd_uniform(rnd_gen), rnd_uniform(rnd_gen), rnd_uniform(rnd_gen);
    }

    //generate sensor measurements and add normal noise to them
    for (unsigned int ii=0; ii<np; ii++)
    {
        noise << rnd_normal(rnd_gen), rnd_normal(rnd_gen), rnd_normal(rnd_gen); 
        measured_points.block<3,1>(0,ii) = true_orientation.inverse()*map_points.block<3,1>(0,ii) + noise; 
    }

    //create outliers
    for (unsigned int ii=0; ii<np; ii++)
    {
        if ( rnd_uniform(rnd_gen) < outlier_ratio ) //outlier case
        {
            measured_points.block<3,1>(0,ii) << measured_points.block<3,1>(0,ii)*rnd_uniform(rnd_gen); 
        }
    }

//***************************************************************

//*************** CERES PROBLEM *********************************

    // Declare the ceres problem.
    ceres::Problem problem;
    
    //declare a pointer to Eigen-Quaternion parameterization
    ceres::LocalParameterization *eigen_quaternion_parameterization = new ceres::EigenQuaternionParameterization;

    //Add constraints to the problem, according user params: use_loss_function and diff_type
    for (unsigned int ii=0; ii<np; ii++)
    {
        //add a residual block for each constraint
        problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<pointConstraint,3,4>(new pointConstraint(measured_points.block<3,1>(0,ii),ii,&map_points)),
            new ceres::CauchyLoss(0.5), 
            optimized_orientation.coeffs().data() );    
    }

    // Apply the parameterization over the 4 quaternion parameters
    problem.SetParameterization(optimized_orientation.coeffs().data(), eigen_quaternion_parameterization);

    //solve: estimate rotation
    ceres::Solver::Options options;
    options.minimizer_type = ceres::TRUST_REGION;
    options.linear_solver_type = ceres::DENSE_QR;    
    options.max_num_iterations = 100;
    options.function_tolerance = 1e-9;
    options.gradient_tolerance = 1e-13;
    options.parameter_tolerance = 1e-11; 
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary); 

    //print results
    std::cout << summary.FullReport() << std::endl;
    std::cout << "true_orientation : " << true_orientation.coeffs().transpose() << std::endl;
    std::cout << "optimized_orientation : " << optimized_orientation.coeffs().transpose() << std::endl;
    
    //exit
    return 0;
}
