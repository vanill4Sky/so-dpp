#include "curses_wrapper.hpp"

#include <ncurses.h>

dpp::curses_wrapper::curses_wrapper()
    : main_window{ initscr() }
{			
    raw();				
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_colorpairs();
}

dpp::curses_wrapper::~curses_wrapper()
{
    echo();
    cbreak();
    endwin();				
}

dpp::window& dpp::curses_wrapper::get_main_window()
{
    return main_window;
}

dpp::window dpp::curses_wrapper::make_window(utils::vec2<size_t> size, utils::vec2<size_t> pos) const
{
    return dpp::window(main_window, size, pos);
}

void dpp::curses_wrapper::init_colorpairs() const 
{
    for (int bg{ 0 }; bg <= 7; ++bg)
    {
        for (int fg{ 0 }; fg <= 7; ++fg)
        {
            auto id = dpp::curses_wrapper::color_pair_id(fg, bg);
            init_pair(id, fg, bg);
        }
    }
}

unsigned int dpp::curses_wrapper::color_pair_id(unsigned int fg, unsigned int bg)
{
    fg &= 7;
    bg &= 7;

    return (bg << 3) | fg;
}