
//std includes
#include <iostream>
#include <random>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

//class holding a point constraint and definig an Auto diff cost function as a templated const operator ()
class pointConstraint
{
    private:
        unsigned int point_id_; //point id
        Eigen::Vector2d point_image_; //the point measured in image coordinates [pixels]
        Eigen::Vector3d point_base_; //the point measured in robot base frame [m]
        Eigen::Matrix3d K_; //intrinsics matrix
        Eigen::MatrixXd P_; //intrinsics matrix * transform of the camera ([I | 0] in this case)
    
    public: 
        pointConstraint(unsigned int _id, 
                        const Eigen::Vector2d & _point_image, 
                        const Eigen::Vector3d & _point_base,
                        const Eigen::Matrix3d & _K ) :
            point_id_(_id), 
            point_image_(_point_image),
            point_base_(_point_base),
            K_(_K),
            P_(3,4)
        {
            //build matrix P_. The camera frame is the reference frame, so there is no extra rotation/translation
            P_.block<3,3>(0,0) << K_;
            P_.block<3,1>(0,3) << 0, 0, 0;
        }
        
        ~pointConstraint()
        {
            //
        }
        
        /** \brief Cost function
         * Cost function where: 
         * \param _p is the translation of the base wrt camera. 3-scalar
         * \param _q is the rotation of the base wrt camera. Quaternion. 4-scalar 
         * \param _residual is the 2D returned residual. 
         **/
        template <typename T> bool operator()(const T* const _p, const T* const _q, T* _residual) const 
        {
            //build the transform. Base pose wrt Camera
            Eigen::Transform<T,3,Eigen::Affine> TB_C; 
            Eigen::Quaternion<T> qB_C(_q);//Orientation of Base wrt Camera. Storing order in eigen is qi,qj,qk,qr. 
            TB_C.linear() = qB_C.toRotationMatrix();//build the rotation matrix from the quaternion
            TB_C.translation() << _p[0], _p[1], _p[2]; 

            //compute cost
            Eigen::Matrix<T,4,1> pt_B;//T-typed, 3D, homogeneous point wrt base
            pt_B << T(point_base_(0)), T(point_base_(1)), T(point_base_(2)), T(1.0);
            Eigen::Matrix<T,3,4> P;//T-typed P_ matrix
            P(0,0) = T(P_(0,0)); P(0,1) = T(P_(0,1)); P(0,2) = T(P_(0,2)); P(0,3) = T(P_(0,3));
            P(1,0) = T(P_(1,0)); P(1,1) = T(P_(1,1)); P(1,2) = T(P_(1,2)); P(1,3) = T(P_(1,3));
            P(2,0) = T(P_(2,0)); P(2,1) = T(P_(2,1)); P(2,2) = T(P_(2,2)); P(2,3) = T(P_(2,3));            
            Eigen::Matrix<T,3,1> pt_projected;//T-typed projected pt_B point to image plane. Product P*TB_C*pt_B 
            pt_projected = P*TB_C.matrix()*pt_B;
            _residual[0] = T(point_image_(0)) - pt_projected(0)/pt_projected(2); 
            _residual[1] = T(point_image_(1)) - pt_projected(1)/pt_projected(2); 
            
            //return 
            return true;
        }
};

//main 
int main(int argc, char** argv) 
{

    //user arguments. 
    unsigned int np = 6; //num of points
    
    //initial guess
    Eigen::Vector3d iguess_center(0.332,0.507,1.034);
//     Eigen::Vector3d iguess_center(0.489543, 0.443138,  1.08142);
    Eigen::Matrix3d iguess_rMat;
    iguess_rMat <<  0,1,0,
                    1,0,0,
                    0,0,-1;
    Eigen::Quaterniond iguess_orientation(iguess_rMat); //initial guess orientation of base wrt the camera
    
    //params to be optimized
    Eigen::Vector3d optimized_center(iguess_center); 
    Eigen::Quaterniond optimized_orientation(iguess_orientation);
    
    //required inits for ceres
    google::InitGoogleLogging(argv[0]);


//************** SET MEASUREMENTS *************************
    
    //measured points in the image
    Eigen::MatrixXd points_image(2,np);
    points_image << 644, 617, 680, 785, 737, 908,    
                    36,  274, 152, 202, 327, 160;     
//     points_image << 1095, 986, 1130, 873, 717, 679, 631, 681, 725, 680,
//                     414,  517, 561,  482, 424, 465, 492, 513, 543, 488;
//     points_image << 873, 717, 679, 631, 681, 725, 680,
//                     482, 424, 465, 492, 513, 543, 488;

    //measured points wrt the robot base                    
    Eigen::MatrixXd points_base(3,np); 
    points_base << -0.600, -0.547, -0.573, -0.562, -0.534, -0.573,
                   -0.484, -0.492, -0.475, -0.455, -0.46,  -0.427,
                   0.658,  0.66,   -0.67,  0.669,  0.666,  0.657; 
//     points_base <<  -0.504, -0.457, -0.439, -0.465, -0.493, -0.470, -0.457, -0.446, -0.430, -0.460,
//                     -0.282, -0.33,  -0.267, -0.358, -0.427, -0.446, -0.471, -0.444, -0.42,  -0.44, 
//                     0.334,  0.32,   0.335,  0.211,  0.1789, 0.188,  0.179,  0.188,  0.179,  0.181;
//     points_base <<  -0.465, -0.493, -0.470, -0.457, -0.446, -0.430, -0.460,
//                     -0.358, -0.427, -0.446, -0.471, -0.444, -0.42,  -0.44, 
//                     0.211,  0.1789, 0.188,  0.179,  0.188,  0.179,  0.181;
                   
    Eigen::Matrix3d matrixK; 
    matrixK <<  1614.930815, 0,             651.174560,
                0,           1616.359896,   360.512415,
                0,           0,             1;
    
//***************************************************************

//*************** CERES PROBLEM *********************************

    // Declare the ceres problem.
    ceres::Problem problem;
    
    //Add constraints to the problem
    for (unsigned int ii=0; ii<np; ii++)
    {
        //add a residual block for each measurement
        problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<pointConstraint,2,3,4>(new pointConstraint(ii, points_image.block<2,1>(0,ii), points_base.block<3,1>(0,ii), matrixK ) ),
            //new ceres::CauchyLoss(0.5), 
            nullptr,
            optimized_center.data(),
            optimized_orientation.coeffs().data() );    
    }

    // Apply the parameterization over the 4 quaternion parameters
    ceres::LocalParameterization *eigen_quaternion_parameterization = new ceres::EigenQuaternionParameterization;    
    problem.SetParameterization(optimized_orientation.coeffs().data(), eigen_quaternion_parameterization);

    //solve: estimate transform of base wrt camera
    ceres::Solver::Options options;
    options.minimizer_type = ceres::TRUST_REGION; //ceres::LINE_SEARCH
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
    std::cout << "iguess_center Base wrt camera : " << iguess_center.transpose() << std::endl;
    std::cout << "iguess_orientation Base wrt camera: " << iguess_orientation.coeffs().transpose() << std::endl;    
    std::cout << "optimized_center Base wrt camera : " << optimized_center.transpose() << std::endl;
    std::cout << "optimized_orientation Base wrt camera: " << optimized_orientation.coeffs().transpose() << std::endl;
    
    //convert to homogeneous matrix
    Eigen::Affine3d TB_C, TC_B;
    TB_C.linear() = optimized_orientation.toRotationMatrix();
    TB_C.translation() = optimized_center;
    TC_B = TB_C.inverse(); 
    Eigen::Vector3d transC_B; 
    transC_B << TC_B.matrix()(0,3),TC_B.matrix()(1,3),TC_B.matrix()(2,3);
    Eigen::Quaterniond quatC_B(TC_B.linear());
    std::cout << std::endl;
    std::cout << "transC_B : " << transC_B.transpose() << std::endl;
    std::cout << "quatC_B: " << quatC_B.coeffs().transpose() << std::endl;
    
    
    //exit
    return 0;
}
