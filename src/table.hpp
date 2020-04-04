#pragma once

#include <atomic>
#include <vector>
#include <stddef.h>

#include "fork.hpp"

namespace dpp
{

struct table
{
    table(size_t philosophers_count);
    
    std::atomic<bool> ready;
    std::vector<dpp::fork> forks;
};


}
