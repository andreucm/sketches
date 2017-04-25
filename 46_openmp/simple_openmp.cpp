/**hello world openmp*/

#include <iostream>
#include <vector>
#include <sys/time.h>
#include <omp.h>

// https://www.dartmouth.edu/~rc/classes/intro_openmp/what_is_openmp.html

int main()
{
    unsigned int ii_max = 10000; 
    std::vector<double> v_in(ii_max); 
    std::vector<double> v_out(ii_max); 
    timeval t1,t2;
    double dT; 
    
    //loop 
    gettimeofday(&t1, NULL); 
#pragma omp parallel for
    for(unsigned int ii=1; ii<ii_max; ii++)
    {
        v_out[ii] = v_in[ii]*v_in[ii-1]; 
    }
    gettimeofday(&t2, NULL); 
    dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
    std::cout << "Delay on computing loop with openmp = " << dT << std::endl;
    
    gettimeofday(&t1, NULL); 
    for(unsigned int ii=1; ii<ii_max; ii++)
    {
        v_out[ii] = v_in[ii]*v_in[ii-1]; 
    }
    gettimeofday(&t2, NULL); 
    dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
    std::cout << "Delay on computing loop without openmp = " << dT << std::endl;
    
    int iam, nt;
#pragma omp parallel default(shared) private(iam,nt)
    {
        iam = omp_get_thread_num();
        nt =  omp_get_num_threads();
        for(unsigned int ii=1; ii<ii_max; ii++)
        {
            v_out[ii] = iam; 
        }
        std::cout << std::endl; 
        std::cout << "nt: " << nt << std::endl; 
    }
    for(unsigned int ii=1; ii<ii_max; ii++)
    {
        std::cout << v_out[ii] << ", ";
    }
    
        
    //return
    return 0;
}
