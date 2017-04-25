
//std 
#include <iostream>

//eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

int main(int argc, char *argv[])
{
//     Eigen::Transform<double,3,Eigen::Affine> T_s_b;
    Eigen::Affine3d T_s_b;     
    
//RAW matrix entry    
    //rotation about Y of alpha, and translation to 1,2,3
    double alpha = 30*M_PI/180.; 
    T_s_b.matrix() << cos(alpha),  0, sin(alpha), 1, 
                   0,           1, 0,          2,
                   -sin(alpha), 0, cos(alpha), 3,
                   0          , 0, 0,          1;

    //transform a point wrt S to a point wrt B
    Eigen::Vector3d p_s(1,1,1);
    Eigen::Vector3d p_b;
    p_b = T_s_b*p_s; 
    std::cout << "T_s_b: " << T_s_b.matrix() <<std::endl;
    std::cout << "p_b: " << p_b.transpose() <<std::endl;
    
//AXIS-ANGLE matrix entry
    T_s_b.setIdentity(); //reset
    T_s_b = Eigen::AngleAxisd(alpha, Eigen::Vector3d::UnitY()); 
    T_s_b.translation() << 1,2,3; 
    p_b = T_s_b*p_s; 
    std::cout << "T_s_b: " << T_s_b.matrix() <<std::endl;
    std::cout << "p_b: " << p_b.transpose() <<std::endl;    
    
//Copy operator
    Eigen::Transform<double,3,Eigen::Affine> T1; 
    T1 = T_s_b; 
    std::cout << "T1:\n" << T1.matrix() <<std::endl;
    
//copy constructor    
    Eigen::Transform<double,3,Eigen::Affine> T2(T1);
    std::cout << "T2:\n" << T2.matrix() <<std::endl;

//rotate about Z
    Eigen::Quaterniond qt; 
    double angle = M_PI/4.;
    qt.w() = cos(angle/2.); 
    qt.x() = 0.;
    qt.y() = 0.;
    qt.z() = sin(angle/2.);
    T2 = T2*qt;         
    std::cout << "T2 rotated:\n" << T2.matrix() <<std::endl;
    
//invert
    Eigen::Transform<double,3,Eigen::Affine> T3(T2.inverse()); 
    std::cout << "T3=T2 inverted:\n" << T3.matrix() <<std::endl;

//quaternion from matrix    
    Eigen::Matrix3d mat2; 
    mat2 << -1,0,0,0,1,0,0,0,-1;
    Eigen::Quaterniond qt2(mat2);
    std::cout << "qt2: " << qt2.coeffs().transpose() <<std::endl;    
    
//exit    
    return 0;
}
