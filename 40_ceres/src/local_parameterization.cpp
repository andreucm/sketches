

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
        //point measurement
        Eigen::Vector3d point_sensor; //a single point measured in sensor frame
        unsigned int point_id_; //associates the measured point with its id in the map. Just to avoid data association step. 
        std::vector<Eigen::Vector3d> *map_; // a set of points wrt global frame
    
    public: 
        pointConstraint(const Eigen::Vector3d & _point, unsigned int _id) :
            point_(_point),
            point_id_(_id)
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
            
            //build the rotation matrix from the quaternion
            matR = quat.toRotationMatrix();
            
            //move the locally measured point to the global frame
            point_map = matR*(*point_sensor); 
            //TODO check if point_map = quat*(*point_sensor); could also work. 
            
            //compute the residual (squared distance) between the measured point in the global frame and its correpsonding map point 
            //3D residual case
            _residual[0] = T( (*map_)[point_id_].x() ) - point_map.x(); 
            _residual[1] = T( (*map_)[point_id_].y() ) - point_map.y(); 
            _residual[2] = T( (*map_)[point_id_].z() ) - point_map.z(); 
//          //1D resiudal case
//             _residual = ( T( (*map_)[point_id_].x() ) - point_map.x() )^2 + 
//                         ( T( (*map_)[point_id_].y() ) - point_map.y() )^2 + 
//                         ( T( (*map_)[point_id_].z() ) - point_map.z() )^2 ; 

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
    Eigen::Quaterniond true_orientation(1,0,0,0); //true oreintation of sensor wrt map
    Eigen::Quaterniond initial_orientation(1,0,0,0); //initial guess orientation of sensor wrt map 
    bool use_loss_function = true;     
    
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
        measured_points = true_orientation.inverse()*map_points.block<3,1>(0,ii); 
        noise << rnd_normal(rnd_gen), rnd_normal(rnd_gen), rnd_normal(rnd_gen); 
        measured_points.block<3,1>(0,ii) += noise; 
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
    ceres::LocalParameterization *eigen_quaternion_parameterization = new ceres_ext::EigenQuaternionParameterization;

    //Add constraints to the problem, according user params: use_loss_function and diff_type
    for (unsigned int ii=0; ii<np; ii++)
    {
        //add a residual block for each constraint
        problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<pointConstraint,3,4>(new pointConstraint(points.block<3,1>(0,ii),ii)),
            new ceres::CauchyLoss(0.5), 
            optimized_orientation.data() );    
        
        // Apply the parameterization
        problem.SetParameterization(optimized_orientation.data(), eigen_quaternion_parameterization);
    }
    
    
    
    //solve: estimate rotation
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
