#include <iostream>
#include <Eigen/Core>

// import most common Eigen types 
using namespace std;
using namespace Eigen;

class VehicleState
{
      protected: 
            VectorXd *ws_ptr;
            Vector3d v3;
      public: 
            VehicleState(VectorXd *ptr)
            { 
                  ws_ptr = ptr; 
                  //ws_vec.resize(100);
                  //ws_vec << 0,-1,-2,-3,-5,-6,-7,-8,-9;
            };
            
            ~VehicleState(){};
            
//             Vector3d & p()
//             {
//                   v3 = ws_ptr->segment(0,3);
//                   return v3;
//             };
            Vector3d & p()
            {
                  v3 = ws_ptr->segment(0,3);
                  return v3;
            };
};

class WinState
{
      protected: 
            VectorXd ws;

      public: 
            WinState()
            { 
                  ws.resize(10);
                  ws << 0,-1,-2,-3,-5,-6,-7,-8,-9;
            };
            
            ~WinState(){};

            Vector3d p()
            {
                  return ws.segment(0,3);
            };
};


int main(int, char *[])
{
      VectorXd winV;
      VehicleState vs(&winV);
      WinState ws;

      //init
      winV.resize(10);
      winV << 0,1,2,3,5,6,7,8,9;
      
      //access VehicleState
      cout << vs.p().transpose() << endl;
      vs.p() << -10,-11,12;
      cout << vs.p().transpose() << endl;
      
      //access winState
      cout << ws.p().transpose() << endl;
      ws.p() << -10,-11,12;
      cout << ws.p().transpose() << endl;
      
      
      return 0;
}
