//
// Created by blackgeorge on 7/1/19.
//

#include "random_data.h"

std::vector<int> create_random_data(int n)
{
    std::random_device r;
    std::seed_seq      seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937       eng(seed); // a source of random data

    std::uniform_int_distribution<int> dist;
    std::vector<int> v(n);

    generate(begin(v), end(v), bind(dist, eng));
    return v;
}

/*
int main()
{
    auto random_data = create_random_data(100000);

    std::cout << "unsorted: ";
    copy(begin(random_data), end(random_data),
         std::ostream_iterator<int>(std::cout, "\n"));
}
 */