#include "table.hpp"

dpp::table::table(size_t philosphers_count)
    : ready{ false }
    , forks{ philosphers_count }
    {}