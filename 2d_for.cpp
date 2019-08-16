//
// Created by blackgeorge on 7/6/19.
//

#include <mutex>
#include <iostream>
#include <thread>

#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range2d.h>

#include <atomic>                                 // !!!

std::mutex cout_mutex;

int main()
{
    auto N = 3;
    auto M = 4;

    tbb::task_scheduler_init init(4);

    tbb::parallel_for(tbb::blocked_range2d<int>(0, N, 0, M),
        [&](const tbb::blocked_range2d<int>& r)
              {
                  for(int j=r.rows().begin(), j_end=r.rows().end(); j<j_end; j++){
                      for(int k=r.cols().begin(), k_end=r.cols().end(); k<k_end; k++){
                          std::this_thread::sleep_for(std::chrono::seconds(1));
                          cout_mutex.lock();
                          std::cout << std::this_thread::get_id() << ", " << j << ", " << k << std::endl;
                          cout_mutex.unlock();
                      }
                  }
              }
    );
}

