#pragma once

#include <string>
#include <string_view>
#include <thread>
#include <random>

#include "fork.hpp"
#include "table.hpp"

namespace dpp
{

class philosopher
{
public:
    philosopher(std::string name, const dpp::table& table, 
        dpp::fork& left_fork, dpp::fork& right_fork);
    philosopher(const philosopher&) = delete;
    philosopher(philosopher&&) = default;
    ~philosopher();

    philosopher& operator=(const philosopher&) = delete;
    philosopher& operator=(philosopher&&) = default;

    void dine();
    void eat();
    void think();
    void print(std::string_view text);

private:
    const std::string name;
    const dpp::table& table;
    dpp::fork& left_fork;
    dpp::fork& right_fork;
    std::thread thread;
    std::default_random_engine rng;
};

}