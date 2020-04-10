#include "curses_wrapper.hpp"

#include <cmath>

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

void dpp::curses_wrapper::circle(int r) const 
{
    auto win_size = get_size();
    utils::vec2<int> s{ win_size.x / 2, win_size.y / 2 };
    const auto pow2 = [](int a) { return a * a; };
    const auto pr = 2.0f;

    for (int row = s.y - r; row <= s.y + r; ++row)
    {
        for (int col = s.x - r * pr; col <= s.x + r * pr; ++col) 
        {
            auto phi = utils::cartesian_to_polar<float>(
                (col - s.x) / pr, row - s.y);

            auto d = std::pow((col - s.x) / static_cast<float>(r * pr), 2) 
                + std::pow(((row - s.y) / static_cast<float>(r)), 2);
            if (d > 0.9f && d < 1.1f)
            {
                const int slices_num = 6;
                for (int i = 0; i < slices_num + 1; ++i)
                {
                    float slice = (2 * M_PI) / slices_num;
                    if (phi >= slice * i && phi <= slice * (i + 1))
                    {
                        std::string sym{ "a" };
                        sym[0] += i;
                        print(sym, row, col);
                        break;
                    }
                }                
            }
        }
    }
    update();
}

// void dpp::curses_wrapper::print(std::string text, int pos_x, int pos_y) const
// {
//     mvprintw(pos_x, pos_y, text.c_str());
// }
