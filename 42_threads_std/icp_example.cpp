#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <chrono>

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

int main()
{
	auto t0 = std::chrono::high_resolution_clock::now();

	uint64_t value = 10;
	std::vector< std::future<uint64_t> > icp_tasks(value);
	for(int i = 0; i < value; i++)
	{
		icp_tasks.at(i) = std::async(std::launch::async, &icp_procedure, i+1);
	}

	// sum all the futures
	// it'll wait for the promises, so wait for everyone to finish
	// PLUS: no need for global shared variable
	std::vector<uint64_t> icp_results;
	for(auto& t : icp_tasks)
	{
		icp_results.push_back( t.get() );
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);

	std::cout << std::endl << "----------------------------------------"<< std::endl;
	std::cout << "Total time for all ICPs(nanoseconds) = " << dt.count() << std::endl;
	int i = 0;
	for(auto r : icp_results)
	{
		std::cout << "ICP " << i+1 << " RESULT = " << r << std::endl;
		i++;
	}

	return 0;
}
