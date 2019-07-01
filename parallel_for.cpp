//
// Created by blackgeorge on 7/1/19.
//

#include <iostream>
#include <chrono>
#include <thread>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include "random_data.h"

void Foo(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void SerialApplyFoo(std::vector<int>* a, int n)
{
    for (auto i = 0; i < n; ++i)
        Foo((*a)[i]);
}

class ApplyFoo {
    std::vector<int>* const my_a;
public:
    explicit ApplyFoo(std::vector<int>* a): my_a{a} {}

    void operator()(const tbb::blocked_range<int>& r) const {
        std::vector<int>* a = my_a;
        for (int i = r.begin(); i != r.end(); ++i)
            Foo((*a)[i]);
    }
};

void ParallelApplyFoo(std::vector<int>* a, int n)
{
    tbb::parallel_for(tbb::blocked_range<int>(0, n), ApplyFoo(a), tbb::auto_partitioner());
}

int main()
{
    using namespace std::chrono;

    int N = 1000;

    auto a = create_random_data(N);


    auto t0 = high_resolution_clock::now();
    SerialApplyFoo(&a, N);
    auto tf = high_resolution_clock::now();
    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms\n";

    tbb::task_scheduler_init init;

    t0 = high_resolution_clock::now();
    ParallelApplyFoo(&a, N);
    tf = high_resolution_clock::now();
    std::cout << "done " << duration_cast<milliseconds>(tf-t0).count() << " ms\n";

    return 0;
}
