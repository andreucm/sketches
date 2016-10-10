#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
 
void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
 
int main()
{
    int n = 0;
    std::thread t1; // t1 is just a thread object, but not a thread of execution
    std::thread t2(f1, n + 1); // pass by value. In this case a thread of execution starts represented by t2
    std::thread t3(f2, std::ref(n)); // pass by reference. In this case a thread of execution starts represented by t3
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread of execution. 
    t2.join(); //wait for t2
    t4.join(); //wait for t4
    std::cout << "Final value of n is " << n << '\n';
}