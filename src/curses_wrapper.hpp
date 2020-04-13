#pragma once

#include "utils.hpp"
#include "window.hpp"

namespace dpp
{

class curses_wrapper;
class window;

class curses_wrapper 
{
public:
	curses_wrapper();
	~curses_wrapper();

    const dpp::window& get_main_window() const;

    dpp::window make_window(utils::vec2<size_t> size, utils::vec2<size_t> pos) const;
    
    static unsigned int color_pair_id(unsigned int fg, unsigned int bg);

private:
    void init_colorpairs() const;

    const dpp::window main_window;
};

}

/*
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
*/