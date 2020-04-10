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
    const size_t id;
    const std::string name;
    const dpp::table& table;
    dpp::fork& left_fork;
    dpp::fork& right_fork;
    std::thread thread;
    std::default_random_engine rng;
    dpp::visualization& visualization;
};

}