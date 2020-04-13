#pragma once

#include <ncurses.h>
#include <cstring>
#include <string_view>

#include "utils.hpp"

namespace dpp
{

class curses_wrapper;

struct scoped_color
{
    scoped_color(const dpp::curses_wrapper& window, unsigned int fg, unsigned int bg);
    ~scoped_color();

private:
    const dpp::curses_wrapper& window;
    const unsigned int fg, bg;
};

class curses_wrapper 
{
public:
	curses_wrapper();
	~curses_wrapper();

	void update() const;

	utils::vec2<int> get_size() const;

	template<typename T>
	void print_centered(T text) const;

	void print(std::string_view text) const;
	void print(std::string_view text, utils::vec2<int> pos) const;
    void print(std::string_view text, int pos_x, int pos_y) const;

    void set_color(unsigned int fg, unsigned int bg) const;
    void unset_color(unsigned int fg, unsigned int bg) const;
    dpp::scoped_color set_scoped_color(unsigned int fg, unsigned int bg) const;

private:
    void init_colorpairs() const;
    unsigned int color_pair_id(unsigned int fg, unsigned int bg) const;

};

}

template<typename T>
void dpp::curses_wrapper::print_centered(T text) const 
{
    auto win_size = get_size();

    int row = win_size.y / 2 - text.size() / 2;
    for (const std::string_view line: text) 
    {
        int col{ win_size.x / 2 - line.size() / 2 };        
        print(line, row, col);
        ++row;
    }
    update();
}