#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include "tbb/tbb.h"


void foo(int* A, int l, int r)
{
    for (auto i = l; i < r; ++i)
        A[i] = A[i]*A[i]*A[i] + 1;
}

std::mutex cout_mutex;

void parallel_foo(int n)
{
   tbb::parallel_for( tbb::blocked_range<int>(0, n),
           [=](const tbb::blocked_range<int>& r)
           {
               for (int i = r.begin(); i != r.end(); ++i) {
                   cout_mutex.lock();
                   std::cout << "i: " << i << std::endl;
                   cout_mutex.unlock();
                   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
               }
           }
   );
}

void heartbeat()
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(true) {
        std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
#pragma clang diagnostic pop
}

void tick(int n)
{
    for (auto i = 0; i < n; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Alive!" << std::endl;
    }
}

int main() {

    using namespace std::chrono;
    std::cout << std::thread::hardware_concurrency() << std::endl;

    auto t0 = high_resolution_clock::now();
//    foo(20);
    auto tf = high_resolution_clock::now();
    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms\n";

    t0 = high_resolution_clock::now();
    parallel_foo(8);
    tf = high_resolution_clock::now();
    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms\n";

    std::thread t {tick, 10};
    t.join();

    return 0;
}