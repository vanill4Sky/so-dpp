#include "window.hpp"

#include <utility>
#include <ncurses.h>

#include "curses_wrapper.hpp"

dpp::scoped_color::scoped_color(const dpp::window& window, unsigned int fg, unsigned int bg) 
    : window{window}, fg{fg}, bg{bg}
{ 
    window.set_color(fg, bg);
}

dpp::scoped_color::~scoped_color()
{
    window.unset_color(fg, bg);
}

dpp::window::window(const dpp::window& parent, utils::vec2<size_t> size, utils::vec2<size_t> pos)
    : win_ptr{ subwin(parent.win_ptr, size.y, size.x, pos.y, pos.x) }
{
    box(win_ptr, 0, 0);
    update();
}

dpp::window::window(WINDOW* win_ptr)
    : win_ptr{ win_ptr }
{}

dpp::window::window(dpp::window&& other)
{
    win_ptr = std::exchange(other.win_ptr, nullptr);
}

dpp::window::~window()
{
    // :((
    if(win_ptr != nullptr && win_ptr != stdscr)
    {
        wborder(win_ptr, ' ', ' ', ' ',' ',' ',' ',' ',' ');
        update();
        delwin(win_ptr);
        win_ptr = nullptr;
    }
}

dpp::window& dpp::window::operator=(dpp::window&& other)
{
    win_ptr = std::exchange(other.win_ptr, nullptr);
}

void dpp::window::update() const
{
    wrefresh(win_ptr);
}

void dpp::window::stop() const
{
    wgetch(win_ptr);
}

void dpp::window::print(std::string_view text, int row, int col) const
{
    constexpr int padding{ 1 };
    mvwprintw(win_ptr, row + padding, col + padding, "%s" ,text.data());
}

utils::vec2<int> dpp::window::get_size() const
{
    return { win_ptr->_maxx, win_ptr->_maxy };
}

void dpp::window::set_color(unsigned int fg, unsigned int bg) const
{
    wattron(win_ptr, COLOR_PAIR(dpp::curses_wrapper::color_pair_id(fg, bg)));
}

void dpp::window::unset_color(unsigned int fg, unsigned int bg) const
{
    wattroff(win_ptr, COLOR_PAIR(dpp::curses_wrapper::color_pair_id(fg, bg)));
}

dpp::scoped_color dpp::window::set_scoped_color(unsigned int fg, unsigned int bg) const
{
    return {*this, fg, bg};
}