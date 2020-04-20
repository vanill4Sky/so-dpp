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

    dpp::window& get_main_window();

    dpp::window make_window(utils::vec2<size_t> size, utils::vec2<size_t> pos) const;
    
    static unsigned int color_pair_id(unsigned int fg, unsigned int bg);

private:
    void init_colorpairs() const;

    dpp::window main_window;
};

}

