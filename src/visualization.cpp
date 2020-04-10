#include "visualization.hpp"

#include <algorithm>

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
    window.print(info, info_vertical_offset + id, 0);
    window.update();
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
    window.print(progressbar,
        info_vertical_offset + id,
        progressbar_horizontal_offset);
    window.update();
}