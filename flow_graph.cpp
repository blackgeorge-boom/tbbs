//
// Created by blackgeorge on 8/16/19.
//

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <tbb/task_group.h>
#include "tbb/flow_graph.h"

using namespace tbb::flow;

struct body {
    std::string my_name;
    explicit body(const char *name) : my_name(name) {}
    void operator()(continue_msg) const {
        printf("%s\n", my_name.c_str());
    }
};

void print_num(int n)
{
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "Look: " << n << std::endl;
}

int main()
{
    graph g;

    broadcast_node< continue_msg > start(g);
    continue_node<continue_msg> a(g, body("A"));
    continue_node<continue_msg> b(g, body("B"));
    continue_node<continue_msg> c(g, body("C"));
    continue_node<continue_msg> d(g, body("D"));
    continue_node<continue_msg> e(g, body("E"));

    make_edge(start, a);
    make_edge(start, b);
    make_edge(a, c);
    make_edge(b, c);
    make_edge(c, d);
    make_edge(a, e);

//    for (int i = 0; i < 3; ++i) {
//        start.try_put(continue_msg());
//        g.wait_for_all();
//    }

    tbb::task_group tg;
    for (auto i = 0; i < 4; ++i) {
        tg.run(
               [=]
                {
                    if (i == 0) return 1;
                    print_num(i);
                }
                );
    }
    tg.wait();

    return 0;
}
