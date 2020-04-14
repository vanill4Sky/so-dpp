#include "visualization.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <iomanip>

dpp::visualization::visualization()
    : main_window{ cw.get_main_window() }
    , info_window{ cw.make_window({main_window.get_size().x / 2, 0}, {0, 0}) }
    , table_window{ cw.make_window(
        { main_window.get_size().x / 2 + 1, 0 }, 
        { main_window.get_size().x / 2, 0 }) }
{
    info_window.print_title("Info");
    print_info_header();
    
    table_window.print_title("Table state");
    print_table_header();
}

dpp::visualization& dpp::visualization::getInstance()
{
    static dpp::visualization instance;
    return instance;
}

void dpp::visualization::add_philosopher(size_t id, std::string name)
{
    philosopher_infos.emplace_back(
        std::move(name), dpp::philosopher_state::waiting);
    update_info(id, dpp::philosopher_state::waiting, 0);
    update_progressbar(id, 1.0f);
}

void dpp::visualization::print_info_header() const
{
    std::stringstream ss;
    ss  << "description" << std::setw(label_length - 3) 
        << "progress" << std::setw(progressbar_length + 1)
        << "dinners";
    info_window.print(ss.str(), 0, 0);
    info_window.print(std::string(info_window.get_size().x - 1, '-'), 1, 0);
}

void dpp::visualization::print_table_header() const
{
    // what can I say
    size_t ho{ 0 };
    for (size_t i = 0; i <= static_cast<size_t>(dpp::philosopher_state::other); ++i)
    {
        auto state{ static_cast<dpp::philosopher_state>(i) };
        auto color{ choose_foreground_color(state) };
        auto desc{ choose_state_description(state) };
        auto ct{ table_window.set_scoped_color(color, COLOR_BLACK) };
        table_window.print(desc, 0, ho);
        ho += desc.size() + 1;
    }
    table_window.print(std::string(info_window.get_size().x - 1, '-'), 1, 0);
}

void dpp::visualization::update_info(size_t id, dpp::philosopher_state new_state, size_t dinners_count)
{
    philosopher_infos[id].state = new_state;

    std::scoped_lock term_lock{ this->mutex_terminal };

    update_description(id);
    update_dinners_count(id, dinners_count);
    update_table(id);

    info_window.update();
    table_window.update();
}

void dpp::visualization::update_description(const size_t id) const
{
    auto state_str{ choose_state_description(philosopher_infos[id].state) };
    std::string info{ philosopher_infos[id].name + ": " + state_str };
    if (info.size() <= label_length)
    {
        info += std::string(label_length - info.size(), ' ');
    }

    const auto fg_color{ choose_foreground_color(philosopher_infos[id].state) };
    const auto ci{ info_window.set_scoped_color(fg_color, COLOR_BLACK) };

    info_window.print(info, id + info_vo, 0);
}

void dpp::visualization::update_dinners_count(const size_t id, size_t dinners_count) const
{
    info_window.print(
        std::to_string(dinners_count), 
        id + info_vo, 
        label_length + progressbar_length + 2);
}

void dpp::visualization::update_table(const size_t id)
{
    const auto r{ std::max( static_cast<int>(philosopher_infos.size()), 5 ) };

    const auto win_size{ table_window.get_size() };
    const utils::vec2<int> s( win_size.x / 2, r + table_vo );

    const auto fg_color{ choose_foreground_color(philosopher_infos[id].state) };
    const auto ct{ table_window.set_scoped_color(fg_color, COLOR_BLACK) };
    
    draw_slice(id, r, s);
}

void dpp::visualization::draw_slice(size_t id, const size_t r, const utils::vec2<int> s) const
{
    const auto pr = 2.0f; //pixel ratio

    for (int row = s.y - r; row <= static_cast<int>(s.y + r); ++row)
    {
        for (int col = s.x - r * pr; col <= static_cast<int>(s.x + r * pr); ++col) 
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
                    table_window.print(sym, row, col);
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
        case dpp::philosopher_state::waiting:
            fg_color = COLOR_YELLOW;
            break;
        default:
            fg_color = COLOR_WHITE;
    }

    return fg_color;
}

std::string dpp::visualization::choose_state_description(dpp::philosopher_state state) const
{
    std::string state_description;
    switch (state)
    {
    case philosopher_state::waiting:
        state_description = "waiting";
        break;
    case philosopher_state::eating:
        state_description = "eating";
        break;
    case philosopher_state::thinking:
        state_description = "thinking";
        break;
    case philosopher_state::finish:
        state_description = "done";
        break;
    default:
        state_description = "other";
    }

    return state_description;
}

std::string dpp::visualization::make_progressbar(const size_t length, const float value)
{
    assert(length > 2);
    assert(value >= 0.0f);

    auto inner_length{ length - 2 };
    
    size_t progress{ static_cast<size_t>(std::ceil(inner_length * value)) };
    std::string progressbar{
        "[" + std::string(progress, '=') +
        std::string(inner_length - progress, ' ') + "]"};

    return progressbar;
}

void dpp::visualization::update_progressbar(size_t id, float value)
{
    auto progressbar{ make_progressbar(progressbar_length, value) };

    std::scoped_lock term_lock{ this->mutex_terminal };

    info_window.print(progressbar,
        id + info_vo,
        label_length);
    info_window.update();
}