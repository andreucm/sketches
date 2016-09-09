
//std includes
#include <iostream>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"


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

    //required inits for ceres
    google::InitGoogleLogging(argv[0]);

    // The variable to solve for with its initial value.
    double initial_x = 5.0;
    double x = initial_x;

    // Build the problem.
    ceres::Problem problem;

    // Set up the only cost function (also known as residual). This uses auto-differentiation to obtain the derivative (jacobian).
    ceres::CostFunction* cost_function = new ceres::AutoDiffCostFunction<MyCostFunctor, 1, 1>(new MyCostFunctor);
    problem.AddResidualBlock(cost_function, nullptr, &x);

    // Run the solver!
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    //print results
    std::cout << summary.BriefReport() << "\n";
    std::cout << "x : " << initial_x << " -> " << x << "\n";
    
    //exit
    return 0;
}
