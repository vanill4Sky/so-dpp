#include "philosopher.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

std::mutex mutex_waiter;

dpp::philosopher::philosopher(size_t id, std::string name, const dpp::table& table, 
    dpp::fork& left_fork, dpp::fork& right_fork, 
    int thinking_duration, int eating_duration, 
    dpp::visualization& visualization)
    : id{ id } 
    , name{ std::move(name) }
    , table{ table }
    , left_fork{ left_fork }
    , right_fork{ right_fork }
    , thread{ &dpp::philosopher::dine, this }
    , rng{ std::random_device{}() }
    , visualization{ visualization }
    , dinners_count{ 0 }
    , thinking_duration{ thinking_duration }
    , eating_duration{ eating_duration }
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
    visualization.update_info(id, dpp::philosopher_state::thinking, dinners_count);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(thinking_duration));
        visualization.update_progressbar(id, static_cast<float>(i) / sleep_period);
    }
    visualization.update_progressbar(id, 1.0f);
}

void dpp::philosopher::eat()
{
    std::unique_lock lock_waiter{ mutex_waiter };
    std::scoped_lock forks_lock{ right_fork.mutex, left_fork.mutex };
    lock_waiter.unlock();

    visualization.update_info(id, dpp::philosopher_state::eating, dinners_count);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (int i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(eating_duration));
        visualization.update_progressbar(id, static_cast<float>(i) / sleep_period);
    }
    visualization.update_progressbar(id, 1.0f);
    
    ++dinners_count;

    visualization.update_info(id, dpp::philosopher_state::finish, dinners_count);
}