#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <future>
#include <chrono>

// just a cleaner impmlementation of the mutex
std::atomic<uint64_t>  accum(0);
void square(uint64_t x)
{
	accum += x*x;
}

int main()
{
	auto t0 = std::chrono::high_resolution_clock::now();

	uint64_t value = 10000;
	std::vector< std::thread > tasks(value);
	for(uint64_t i = 0; i < value; i++)
	{
		tasks.at(i) = std::thread(&square, i+1);
	}

	// wait for everyone to finish, with atomic
	for(auto& t : tasks)
	{
		t.join();
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	auto dt = (t1-t0).count();

	std::cout << "Finished with accumulator = " << accum << std::endl;
	std::cout << "Total time (nanoseconds) = " << dt << std::endl;

	return 0;
}
