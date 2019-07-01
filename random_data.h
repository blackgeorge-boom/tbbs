//
// Created by blackgeorge on 7/1/19.
//

#ifndef TBBS_RANDOM_DATA_H
#define TBBS_RANDOM_DATA_H

#include <random>     // mt19937 and uniform_int_distribution
#include <algorithm>  // generate
#include <vector>     // vector
#include <iterator>   // begin, end, and ostream_iterator
#include <functional> // bind
#include <iostream>   // cout

std::vector<int> create_random_data(int n);

#endif //TBBS_RANDOM_DATA_H
