
//std 
#include <iostream>

//eigen
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

int main(int argc, char *argv[])
{
    Eigen::Transform<double,3,Eigen::Affine> T_s_b; 
    
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
    
    return 0;
}
