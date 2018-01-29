cmake_minimum_required (VERSION 2.6)
project (threads_tests)

add_definitions(-std=c++11)
find_package(Threads)

add_executable(threads_1
	threads_1.cpp
)
target_link_libraries(threads_1
	${CMAKE_THREAD_LIBS_INIT}
)

add_executable(atomic_sum
	atomic_sum.cpp
)
target_link_libraries(atomic_sum
	${CMAKE_THREAD_LIBS_INIT}
)

add_executable(async_sum
	async_sum.cpp
)
target_link_libraries(async_sum
	${CMAKE_THREAD_LIBS_INIT}
)

add_executable(icp_example
	icp_example.cpp
)
target_link_libraries(icp_example
	${CMAKE_THREAD_LIBS_INIT}
)
