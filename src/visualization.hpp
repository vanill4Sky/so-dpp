#pragma once

#include <mutex>

#include "curses_wrapper.hpp"

namespace dpp
{

class visualization
{
public:
    static visualization& getInstance()
    {
        static visualization instance;
        return instance;
    }

    visualization(dpp::visualization const&) = delete;
    void operator=(visualization const&) = delete;

private:
    visualization();

private:
    std::mutex mutexTerminal;
    curses_wrapper window;
};

}