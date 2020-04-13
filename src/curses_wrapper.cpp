#include "curses_wrapper.hpp"

#include <cmath>
#include <string>

dpp::curses_wrapper::curses_wrapper()
{
    initscr();			
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

void dpp::curses_wrapper::update() const 
{
    refresh();
}

utils::vec2<int> dpp::curses_wrapper::get_size() const
{
    return { stdscr->_maxx, stdscr->_maxy };
}

void dpp::curses_wrapper::print(std::string_view text) const
{
    printw(text.data());
}

void dpp::curses_wrapper::print(std::string_view text, utils::vec2<int> pos) const
{
    mvprintw(pos.y, pos.x, text.data());
}

void dpp::curses_wrapper::print(std::string_view text, int row, int col) const
{
    mvprintw(row, col, text.data());
}

void dpp::curses_wrapper::init_colorpairs() const 
{
    for (int bg{ 0 }; bg <= 7; ++bg)
    {
        for (int fg{ 0 }; fg <= 7; ++fg)
        {
            auto id = color_pair_id(fg, bg);
            init_pair(id, fg, bg);
        }
    }
}

unsigned int dpp::curses_wrapper::color_pair_id(unsigned int fg, unsigned int bg) const
{
    fg &= 7;
    bg &= 7;

    return (bg << 3) | fg;
}

void dpp::curses_wrapper::set_color(unsigned int fg, unsigned int bg) const
{
    attron(COLOR_PAIR(color_pair_id(fg, bg)));
}

void dpp::curses_wrapper::unset_color(unsigned int fg, unsigned int bg) const
{
    attroff(COLOR_PAIR(color_pair_id(fg, bg)));
}

dpp::scoped_color dpp::curses_wrapper::set_scoped_color(unsigned int fg, unsigned int bg) const
{
    return {*this, fg, bg};
}

dpp::scoped_color::scoped_color(const dpp::curses_wrapper& window, unsigned int fg, unsigned int bg) 
    : window{window}, fg{fg}, bg{bg}
{ 
    window.set_color(fg, bg);
}

dpp::scoped_color::~scoped_color()
{
    window.unset_color(fg, bg);
}

// void dpp::curses_wrapper::print(std::string text, int pos_x, int pos_y) const
// {
//     mvprintw(pos_x, pos_y, text.c_str());
// }
