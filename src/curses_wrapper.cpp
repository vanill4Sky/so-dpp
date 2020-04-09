#include "curses_wrapper.hpp"

#include <ncurses.h>

dpp::curses_wrapper::curses_wrapper()
{
    initscr();			
    raw();				
    keypad(stdscr, TRUE);
    noecho();
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

void dpp::curses_wrapper::print(const std::string& text) const
{
    printw(text.c_str());
}

void dpp::curses_wrapper::print(std::string text, utils::vec2<int> pos) const
{
    mvprintw(pos.x, pos.y, text.c_str());
}

// void dpp::curses_wrapper::print(std::string text, int pos_x, int pos_y) const
// {
//     mvprintw(pos_x, pos_y, text.c_str());
// }
