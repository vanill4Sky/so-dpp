#pragma once

#include <string>
#include <thread>
#include <random>

#include "fork.hpp"
#include "table.hpp"
#include "visualization.hpp"

namespace dpp
{

class philosopher
{
public:
    philosopher(size_t id, std::string name, const dpp::table& table, 
        dpp::fork& left_fork, dpp::fork& right_fork,
        int thinking_duration, int eating_duration,
        dpp::visualization& visualization);
    philosopher(const philosopher&) = delete;
    philosopher(philosopher&&) = default;
    ~philosopher();

    philosopher& operator=(const philosopher&) = delete;
    philosopher& operator=(philosopher&&) = default;

    void dine();
    void eat();
    void think();

private:
    size_t id;
    std::string name;
    const dpp::table& table;
    dpp::fork& left_fork;
    dpp::fork& right_fork;
    std::thread thread;
    std::default_random_engine rng;
    dpp::visualization& visualization;
    size_t dinners_count;
    int thinking_duration;
    int eating_duration;
};

}