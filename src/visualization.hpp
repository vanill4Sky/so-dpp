#pragma once

#include <mutex>
#include <vector>

#include "curses_wrapper.hpp"
#include "utils.hpp"

namespace dpp
{

enum class philosopher_state
{
    thinking = 0,
    waiting,
    eating,
    finish,
    other
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
    void update_info(size_t id, dpp::philosopher_state new_state, size_t dinners_count);
    void update_progressbar(size_t id, float value);

    void halt(){ info_window.stop(); }

private:
    visualization();

    void print_info_header() const;
    void print_table_header() const;
    void update_description(const size_t id) const;
    void update_dinners_count(const size_t id, size_t dinners_count) const;
    void update_table(const size_t id);
    void draw_table(const size_t r, const utils::vec2<int> s) const;
    void draw_slice(size_t id, const size_t r, const utils::vec2<int> s) const;
    unsigned int choose_foreground_color(dpp::philosopher_state state) const;
    std::string choose_state_description(dpp::philosopher_state state) const;
    std::string make_progressbar(const size_t length, const float value);
    std::string make_philosopher_symbol(size_t id) const;

private:
    const size_t info_vo = 2; // info vertical offset
    const size_t table_vo = 2; // table vertical offset
    const size_t label_length = 25;
    const size_t progressbar_length = 15;

    std::mutex mutex_terminal;
    const curses_wrapper cw;
    const dpp::window& main_window;
    const dpp::window info_window;
    const dpp::window table_window;
    //const dpp::window table_window;
    std::vector<dpp::philosopher_info> philosopher_infos;
};

}