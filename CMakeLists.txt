cmake_minimum_required(VERSION 3.0)
project(tbbs)

list(APPEND CMAKE_MODULE_PATH "/home/blackgeorge/CLionProjects/FindTBB")

find_package(TBB COMPONENTS tbbmalloc tbbmalloc_proxy tbb_preview)

add_executable(main main.cpp)
add_executable(simple simple.cpp)
add_executable(parallel_for parallel_for.cpp random_data.h random_data.cpp)
add_executable(random_data random_data.cpp random_data.h)

target_link_libraries(main tbb)
target_link_libraries(simple tbb)
target_link_libraries(parallel_for tbb)
target_link_libraries(random_data tbb)
