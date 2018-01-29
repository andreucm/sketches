#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <chrono>

// do costly operation in async (promise/future)
uint64_t square(uint64_t x)
{
	return x*x;
}

int main()
{
	auto t0 = std::chrono::high_resolution_clock::now();

	uint64_t value = 10000;
	std::vector< std::future<uint64_t> > tasks(value);
	for(int i = 0; i < value; i++)
	{
		tasks.at(i) = std::async(std::launch::async, &square, i+1);
	}

	// sum all the futures
	// it'll wait for the promises, so wait for everyone to finish
	// PLUS: no need for global shared variable
	uint64_t result = 0;
	for(auto& t : tasks)
	{
		result += t.get();
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto dt = (t1-t0).count();

	std::cout << "Finished with accumulator = " << result << std::endl;
	std::cout << "Total time (nanoseconds) = " << dt << std::endl;

	return 0;
}
