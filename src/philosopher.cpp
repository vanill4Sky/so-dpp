#include "philosopher.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

std::mutex g_lockprint;

dpp::philosopher::philosopher(std::string name, const dpp::table& table, 
    dpp::fork& left_fork, dpp::fork& right_fork)
    : name{ std::move(name) }
    , table{ table }
    , left_fork{ left_fork }
    , right_fork{ right_fork }
    , thread{ &dpp::philosopher::dine, this }
    , rng{ std::random_device{}() }
    {}

dpp::philosopher::~philosopher()
{
    thread.join();
}

void dpp::philosopher::dine()
{
    while (!table.ready);

    do
    {
        think();
        eat();
    } while (table.ready);
}

void dpp::philosopher::think()
{
    static thread_local std::uniform_int_distribution dist{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 100));

    print("is thinking.");
}

void dpp::philosopher::eat()
{
    std::lock(left_fork.mutex, right_fork.mutex);
    std::lock_guard left_fork_lock(left_fork.mutex, std::adopt_lock);
    std::lock_guard right_fork_lock(right_fork.mutex, std::adopt_lock);

    print("start eating.");

    static thread_local std::uniform_int_distribution dist{1, 10};
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

    print("finished eating.");
}

void dpp::philosopher::print(std::string_view text)
{
    std::lock_guard cout_lock(g_lockprint);
    std::cout << name << " " << text << '\n';
}