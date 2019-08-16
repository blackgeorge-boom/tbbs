//
// Created by blackgeorge on 7/3/19.
//

#include <mutex>
#include <iostream>
#include <thread>

#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include <atomic>                                 // !!!

std::mutex cout_mutex;

int main()
{
    auto N = 8;

    tbb::task_scheduler_init init(4);

    std::atomic<int> monotonic_begin{7};                 // !!!

    tbb::parallel_for(tbb::blocked_range<int>(0, N),
        [&](const tbb::blocked_range<int>& r)
              {
                  int s = static_cast<int>(r.size());    // !!!
                  int b = monotonic_begin.fetch_sub(s);  // !!!
                  int e = b + s;                         // !!!
                  for (int j = b; j < e; ++j) {          // !!!
                      // Compute A[j]
                      std::this_thread::sleep_for(std::chrono::seconds(1));
                      cout_mutex.lock();
                      std::cout << std::this_thread::get_id() << ", " << j << std::endl;
                      cout_mutex.unlock();
                  }
              }
    );
}
