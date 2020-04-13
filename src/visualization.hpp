#pragma once

#include <mutex>
#include <vector>

#include "curses_wrapper.hpp"
#include "utils.hpp"

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

    void update_table(const size_t id);
    void draw_table(const size_t r, const utils::vec2<int> s) const;
    void draw_slice(size_t id, const size_t r, const utils::vec2<int> s) const;
    unsigned int choose_foreground_color(dpp::philosopher_state state) const;

private:
    const size_t info_vertical_offset = 2;
    const size_t progressbar_horizontal_offset = 25;
    const size_t progressbar_length = 15;

    std::mutex mutex_terminal;
    const curses_wrapper window;
    std::vector<dpp::philosopher_info> philosopher_infos;
};

}