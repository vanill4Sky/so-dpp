#include "visualization.hpp"

#include <algorithm>

dpp::visualization::visualization()
    : main_window{ cw.get_main_window() }
    , info_window{ cw.make_window({0, 0}, {0, 0}) }
{}

dpp::visualization& dpp::visualization::getInstance()
{
    static dpp::visualization instance;
    return instance;
}

void dpp::visualization::add_philosopher(size_t id, std::string name)
{
    philosopher_infos.emplace_back(
        std::move(name), dpp::philosopher_state::waiting);
    update_info(id, dpp::philosopher_state::waiting);
    update_progressbar(id, 1.0f);
}

void dpp::visualization::update_info(size_t id, dpp::philosopher_state new_state)
{
    philosopher_infos[id].state = new_state;
    std::string state_str;
    switch (new_state)
    {
    case philosopher_state::waiting:
        state_str = "waiting";
        break;
    case philosopher_state::eating:
        state_str = "eating";
        break;
    case philosopher_state::thinking:
        state_str = "thinking";
        break;
    case philosopher_state::finish:
        state_str = "done";
        break;
    default:
        state_str = "???";
    }

    std::string info{ philosopher_infos[id].name + " is " + state_str };
    if (info.size() <= progressbar_horizontal_offset)
    {
        info += std::string(progressbar_horizontal_offset - info.size(), ' ');
    }
    
    std::scoped_lock term_lock{ this->mutex_terminal };
    const auto fg_color{ choose_foreground_color(philosopher_infos[id].state) };
    const auto ci{ info_window.set_scoped_color(fg_color, COLOR_BLACK) };
    info_window.print(info, id, 0);
    update_table(id);
    info_window.update();
}

void dpp::visualization::update_table(const size_t id)
{
    const auto r{ std::max( static_cast<int>(philosopher_infos.size()), 5 ) };

    const auto table_horizontal_offset{ 
        progressbar_horizontal_offset + progressbar_length };
    const auto win_size{ main_window.get_size() };
    const utils::vec2<int> s( 
        win_size.x / 2 + table_horizontal_offset / 2, 
        r + info_vertical_offset / 2);
    
    draw_slice(id, r, s);
}

void dpp::visualization::draw_slice(size_t id, const size_t r, const utils::vec2<int> s) const
{
    const auto pr = 2.0f; //pixel ratio

    for (int row = s.y - r; row <= s.y + r; ++row)
    {
        for (int col = s.x - r * pr; col <= s.x + r * pr; ++col) 
        {
            auto phi = utils::cartesian_to_polar<float>(
                (col - s.x) / pr, row - s.y);

            auto d = std::pow((col - s.x) / static_cast<float>(r * pr), 2) 
                + std::pow(((row - s.y) / static_cast<float>(r)), 2);
            if (d < 1.1f)
            {
                const auto slices_num = philosopher_infos.size();
                float slice = (2 * M_PI) / slices_num;

                if (phi >= slice * id && phi <= slice * (id + 1))
                {
                    std::string sym{ "0" };
                    sym[0] += id;
                    info_window.print(sym, row, col);
                }                
            }
        }
    }
}

unsigned int dpp::visualization::choose_foreground_color(dpp::philosopher_state state) const
{
    unsigned int fg_color;
    switch(state)
    {
        case dpp::philosopher_state::eating:
            fg_color = COLOR_RED;
            break;
        case dpp::philosopher_state::thinking:
            fg_color = COLOR_BLUE;
            break;
        case dpp::philosopher_state::finish:
            fg_color = COLOR_GREEN;
            break;
        default:
            fg_color = COLOR_WHITE;
    }

    return fg_color;
}

void dpp::visualization::update_progressbar(size_t id, float value)
{
    int progress = std::ceil(progressbar_length * value);

    std::string progressbar{
        "[" + 
        std::string(progress, '#') +
        std::string(progressbar_length - progress, '-')
        + "]"};

    std::scoped_lock term_lock{ this->mutex_terminal };
    info_window.print(progressbar,
        id,
        progressbar_horizontal_offset);
    info_window.update();
}