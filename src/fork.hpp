#pragma once

#include <mutex>

namespace dpp
{

struct fork
{
    std::mutex mutex;
};


}