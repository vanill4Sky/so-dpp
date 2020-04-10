#pragma once

#include <mutex>
#include <vector>

#include "curses_wrapper.hpp"

namespace dpp
{

enum class philosopher_state
{
    thinking,
    waiting,
    eating,
    finish
};

struct philosopher_info
{
    philosopher_info(std::string name, philosopher_state state)
        : name{ std::move(name) }, state{ state } 
    {}

    std::string name;
    philosopher_state state;
};

class visualization
{
public:
    static visualization& getInstance();

    visualization(dpp::visualization const&) = delete;
    void operator=(visualization const&) = delete;

    void add_philosopher(size_t id, std::string name);
    void update_info(size_t id, dpp::philosopher_state new_state);
    void update_progressbar(size_t id, float value);

private:
    visualization() {};

private:
    const size_t info_vertical_offset = 2;
    const size_t progressbar_horizontal_offset = 25;
    const size_t progressbar_length = 10;

    std::mutex mutex_terminal;
    const curses_wrapper window;
    std::vector<dpp::philosopher_info> philosopher_infos;
};

}