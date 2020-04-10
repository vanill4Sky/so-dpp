#include "philosopher.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>

std::mutex g_lockprint;

dpp::philosopher::philosopher(size_t id, std::string name, const dpp::table& table, 
    dpp::fork& left_fork, dpp::fork& right_fork, dpp::visualization& visualization)
    : id{ id } 
    , name{ std::move(name) }
    , table{ table }
    , left_fork{ left_fork }
    , right_fork{ right_fork }
    , thread{ &dpp::philosopher::dine, this }
    , rng{ std::random_device{}() }
    , visualization{ visualization }
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
    visualization.update_info(id, dpp::philosopher_state::thinking);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (size_t i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        visualization.update_progressbar(id, static_cast<float>(i) / sleep_period);
    }
    visualization.update_progressbar(id, 1.0f);
}

void dpp::philosopher::eat()
{
    std::scoped_lock forks_lock{ left_fork.mutex, right_fork.mutex };

    visualization.update_info(id, dpp::philosopher_state::eating);

    static thread_local std::uniform_int_distribution dist{ 1, 20 };
    const auto sleep_period{ dist(rng) };
    for (size_t i = 0; i < sleep_period; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        visualization.update_progressbar(id, static_cast<float>(i) / sleep_period);
    }
    visualization.update_progressbar(id, 1.0f);

    visualization.update_info(id, dpp::philosopher_state::finish);
}