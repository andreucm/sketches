#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <chrono>


class ICPer
{
public:
	ICPer(){}
	~ICPer(){}

	void setValue(uint64_t __v)
	{
		value__ = __v;
		icp_tasks__.resize( value__ );
	}

	void compute()
	{
		for(uint64_t i = 0; i < value__; i++)
		{
			icp_tasks__.at(i) = std::async(std::launch::async, &ICPer::icp_procedure, this, i+1);
		}
		
		for(auto& t : icp_tasks__)
		{
			icp_results__.push_back( t.get() );
		}
	}

	void printResults()
	{
		int i = 0;
		for(auto r : icp_results__)
		{
			std::cout << "ICP " << i+1 << " RESULT = " << r << std::endl;
			i++;
		}
	}

protected:
	uint64_t value__;
	// do costly operation in async (promise/future)
	// for instance an icp procedure
	uint64_t icp_procedure(uint64_t x)
	{
		for(int i = 0; i < 10; i++)
		{
			uint64_t a = x*x;
			std::cout << "ICP " << x << ", iteration " << i << "; x*x = " << a << std::endl;
		}
		return x*x;
	}
	std::vector< std::future<uint64_t> > icp_tasks__;
	std::vector<uint64_t> icp_results__;
};



int main()
{
	auto t0 = std::chrono::high_resolution_clock::now();

	uint64_t value = 10;
	ICPer icp;
	icp.setValue( value );
	icp.compute();

	auto t1 = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);

	std::cout << std::endl << "----------------------------------------"<< std::endl;
	std::cout << "Total time for all ICPs(nanoseconds) = " << dt.count() << std::endl;
	icp.printResults();

	return 0;
}
