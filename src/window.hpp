#pragma once

#include <ncurses.h>
#include <string_view>

#include "utils.hpp"

namespace dpp
{

class window;

struct scoped_color
{
    scoped_color(const dpp::window& window, unsigned int fg, unsigned int bg);
    ~scoped_color();

private:
    const dpp::window& window;
    const unsigned int fg, bg;
};

class window
{
public:
    window(const dpp::window&) = delete;
    window(dpp::window&& other);
    ~window();

    dpp::window& operator=(const dpp::window&) = delete;
    dpp::window& operator=(dpp::window&& other);

    void update() const;
    void stop() const;

    void print(std::string_view text, size_t row, size_t col, size_t padding = 1) const;
    void print_centered(std::string_view text, size_t row, size_t padding = 1) const;
    void print_title(std::string_view text) const;

    utils::vec2<size_t> get_size() const;
    void set_color(unsigned int fg, unsigned int bg) const;
    void unset_color(unsigned int fg, unsigned int bg) const;
    dpp::scoped_color set_scoped_color(unsigned int fg, unsigned int bg) const;

    friend class curses_wrapper;

private:
    window(const dpp::window& parent, utils::vec2<size_t> size, utils::vec2<size_t> pos);
    window(WINDOW* win_ptr);

    WINDOW* win_ptr;
};

}