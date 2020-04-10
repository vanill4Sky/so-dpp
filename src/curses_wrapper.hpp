#pragma once

#include <ncurses.h>
#include <string>
#include <cstring>

#include "utils.hpp"

namespace dpp
{

class curses_wrapper 
{
public:
	curses_wrapper();
	~curses_wrapper();

	void update() const;

	utils::vec2<int> get_size() const;

	template<typename T>
	void print_centered(T text) const;

	void print(const std::string& text) const;
	void print(std::string text, utils::vec2<int> pos) const;

    template<typename T>
	void print(T text, int pos_x, int pos_y) const;

    void circle(int r) const;
};

}

template<typename T>
void dpp::curses_wrapper::print_centered(T text) const 
{
    auto win_size = get_size();

    int row = win_size.y / 2 - text.size() / 2;
    for (const auto line: text) 
    {
        int col;
        if constexpr(std::is_same_v<decltype(line), std::string>)
        {
            col = static_cast<int>(win_size.x / 2 - line.size() / 2);
        }
        else
        {
            col = static_cast<int>(win_size.x / 2 - strlen(line) / 2);
        }
        
        print(line, row, col);
        ++row;
    }
    update();
}

template<typename T>
void dpp::curses_wrapper::print(T text, int row, int col) const
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        mvprintw(row, col, text.c_str());
    }
    else
    { 
        mvprintw(row, col, text);
    }
}