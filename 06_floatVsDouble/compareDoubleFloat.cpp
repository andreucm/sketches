
#include <sys/time.h>
#include <iostream>

const int NN = 1000000;

int main(int argc, char *argv[])
{
	int ii;
	double t0,tf;
	double da, dr, d0;
	float fa, fr, f0;
	timeval t1,t2;
	double dT=0;

	gettimeofday(&t1, NULL); 
	for (ii=0;ii<NN;ii++)
	{
		f0=f0+ii;
		fa=fa*f0;
		fr=fr+fa;
	}
	gettimeofday(&t2, NULL); 
	dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
	std::cout << "Delay on computing loop with floats = " << dT << std::endl;
	
	gettimeofday(&t1, NULL); 
	for (ii=0;ii<NN;ii++)
	{
		d0=d0+ii;
		da=da*d0;
		dr=dr+da;
	}
	gettimeofday(&t2, NULL); 
	dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
	std::cout << "Delay on computing loop with doubles = " << dT << std::endl;

/*	gettimeofday(&t1, NULL); 
	for (ii=0;ii<NN;ii++)
	{
		f0=f0+ii;								
		fa=fa*f0;
		fr=fr+fa;
	}
	gettimeofday(&t2, NULL); 
	dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
	std::cout << "Delay on computing loop with floats = " << dT << std::endl;
	
	gettimeofday(&t1, NULL); 
	for (ii=0;ii<NN;ii++)
	{
		d0=d0+ii;
		da=da*d0;
		dr=dr+da;
	}
	gettimeofday(&t2, NULL); 
	dT = (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );
	std::cout << "Delay on computing loop with doubles = " << dT << std::endl;*/
}
