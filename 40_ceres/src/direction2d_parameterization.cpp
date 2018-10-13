//std includes
#include <iostream>
#include <random>
#include <chrono>

//ceres includes
#include "ceres/ceres.h"
#include "glog/logging.h"

//eigen includes
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

class CERES_EXPORT Direction2dParameterization : public ceres::LocalParameterization
{
	public:
		virtual ~Direction2dParameterization() {}

  		virtual bool Plus(const double* _x, const double* _delta, double* _x_plus_delta) const
		{
			double angle = atan2(_x[1],_x[0]);
			_x_plus_delta[0] = cos( angle + _delta[0] );
			_x_plus_delta[1] = sin( angle + _delta[0] );
			return true;
		}

		virtual bool ComputeJacobian(const double* _x, double* _jacobian) const
		{
			double angle = atan2(_x[1],_x[0]);
			_jacobian[0] = -sin( angle );
			_jacobian[1] = cos( angle );
			return true;
		}

  		virtual int GlobalSize() const { return 2; }

  		virtual int LocalSize() const { return 1; }
};

class PointConstraint
{
	protected:
		Eigen::Vector2d point_;

	public:
		PointConstraint(const Eigen::Vector2d & _point) :
			point_(_point)
		{
			point_.normalize(); //keep only the direction, in case it was not normalized
		}

		~PointConstraint()
		{

		}

		template <typename T> bool operator()(const T* const _a, T* _residual) const
        {
			Eigen::Matrix<T,2,1> pt;
			Eigen::Matrix<T,2,1> direction;
			pt << T(point_(0)), T(point_(1));
			direction << _a[0], _a[1];
			//_residual[0] = (direction.dot(pt) - T(1.0)); //results in a bias at the final optimized value
			_residual[0] = atan2(pt(1),pt(0)) - atan2(direction(1),direction(0)); //it works, but could be better to do not use arctan here, since it behave very differently wrt the angle
			_residual[0] = atan2(pt(1),pt(0)) - atan2(direction(1),direction(0));
			return true;
		}
};

int main(int argc, char** argv)
{
    //user arguments.
    unsigned int np = 500; //num of points
    double angle_true = 30*M_PI/180.0; //the true angle from which points are generated [rad]
    double angle_noise_stddev = 0.1; //noise stddev in angle [rad]
	double angle_init_guess = 60.0*M_PI/180.0; //[rad]

    // direction vector to be optimized, initialized with init guess
	Eigen::Vector2d direction_optimized(cos(angle_init_guess), sin(angle_init_guess));

    //required inits for ceres
    google::InitGoogleLogging(argv[0]);

	//************** GERNERATE MEASUREMENTS *************************
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::default_random_engine rnd_generator;
	rnd_generator.seed(tp.time_since_epoch().count());
    std::normal_distribution<double> rnd_normal(0.,angle_noise_stddev);
    Eigen::MatrixXd points(2,np);
    Eigen::Vector3d noise;
	double cos_angle, sin_angle;
    for (unsigned int ii=0; ii<np; ii++)
    {
		cos_angle = cos(angle_true + rnd_normal(rnd_generator));
		sin_angle = sin(angle_true + rnd_normal(rnd_generator));
        points.block<2,1>(0,ii)  << cos_angle, sin_angle;
    }

	//*************** CERES PROBLEM *********************************
    ceres::Problem problem; // Declare the ceres problem.
    ceres::LocalParameterization *angle_parameterization = new Direction2dParameterization;
    for (unsigned int ii=0; ii<np; ii++) //Add constraints to the problem. (add a residual block for each constraint)
    {
        problem.AddResidualBlock(
            new ceres::AutoDiffCostFunction<PointConstraint,1,2>(new PointConstraint(points.block<2,1>(0,ii))),
            nullptr,
            direction_optimized.data() );
    }
    problem.SetParameterization(direction_optimized.data(), angle_parameterization); // Apply the angle parameterization over the 2 angle parameters
    ceres::Solver::Options options;
    options.minimizer_type = ceres::TRUST_REGION;
    options.linear_solver_type = ceres::DENSE_QR;
    options.max_num_iterations = 100;
    options.function_tolerance = 1e-13;
    options.gradient_tolerance = 1e-13;
    options.parameter_tolerance = 1e-11;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);

    // ****************** PRINT RESULTS **************************
    std::cout << summary.FullReport() << std::endl;
    std::cout << "angle_true: " << angle_true*180.0/M_PI << std::endl;
	double angle_optimized = atan2(direction_optimized(1),direction_optimized(0));
    std::cout << "angle_optimized : " << angle_optimized*180.0/M_PI << std::endl;

	//check computing the mean estimator
	Eigen::Vector2d direction_check;
	double dx(0), dy(0);
	for (unsigned int ii=0; ii<np; ii++)
	{
		direction_check << points.block<2,1>(0,ii);
		direction_check.normalize();
		dx += direction_check(0);
		dy += direction_check(1);
	}
	double angle_check = atan2(dy,dx);
	std::cout << "angle_check : " << angle_check*180.0/M_PI << std::endl;

    //exit
    return 0;
}
