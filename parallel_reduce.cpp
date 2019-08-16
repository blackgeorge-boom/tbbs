//
// Created by blackgeorge on 7/3/19.
//

#include <vector>
#include <chrono>

#include "foo.h"
#include "random_data.h"

#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

class SumFoo {
    std::vector<int>* my_a;
public:
    long int sum;

    explicit SumFoo(std::vector<int>* a): my_a{a}, sum{0} {};
    SumFoo(SumFoo& x, tbb::split) : my_a{x.my_a}, sum{0} {};

    void operator()(const tbb::blocked_range<int>& r) {
        std::vector<int>* a = my_a;

        for (auto i = r.begin(); i != r.end(); ++i)
            sum += Foo((*a)[i]);
    }

    void join(const SumFoo& y) { sum += y.sum; }
};

long int SerialSumFoo(std::vector<int>* a, int n)
{
    int sum = 0;

    for (auto x : *a)
        sum += Foo(x);

    return sum;
}

long int ParallelSumFoo(std::vector<int>* a, int n)
{
    SumFoo sf(a);
    tbb::parallel_reduce(tbb::blocked_range<int>(0, n, 1000), sf);
    return sf.sum;
}

int main()
{
    int N = 100000000;
    auto a = create_random_data(N);

    auto t0 = std::chrono::high_resolution_clock::now();
    long res = SerialSumFoo(&a, N);
    auto tf = std::chrono::high_resolution_clock::now();

    std::cout << "Sum is: " << res << std::endl;
    std::cout << "Done in: " << std::chrono::duration_cast<std::chrono::milliseconds>(tf-t0).count() << " ms" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    res = ParallelSumFoo(&a, N);
    tf = std::chrono::high_resolution_clock::now();

    std::cout << "Sum is: " << res << std::endl;
    std::cout << "Done in: " << std::chrono::duration_cast<std::chrono::milliseconds>(tf-t0).count() << " ms" << std::endl;
}
