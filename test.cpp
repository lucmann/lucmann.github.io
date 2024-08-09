// Copyright (C) 2024 Luc Ma
//
// SPDX-License-Identifier: GPL-2.0
#include <atomic>
#include <cassert>
#include <thread>
 
std::atomic<bool> x = {false};
std::atomic<bool> y = {false};
std::atomic<int> z = {0};
 
void write_x()
{
    x.store(true, std::memory_order_acq_rel);
}
 
void write_y()
{
    y.store(true, std::memory_order_acq_rel);
}
 
void read_x_then_y()
{
    while (!x.load(std::memory_order_acq_rel))
        ;
    if (y.load(std::memory_order_acq_rel))
        ++z;
}
 
void read_y_then_x()
{
    while (!y.load(std::memory_order_acq_rel))
        ;
    if (x.load(std::memory_order_acq_rel))
        ++z;
}
 
int main()
{
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join(); b.join(); c.join(); d.join();
    assert(z.load() != 0); // will never happen
}
