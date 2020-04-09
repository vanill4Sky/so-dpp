#include <iostream>
#include <ncurses.h>
#include <thread>
#include <array>

#include "fork.hpp"
#include "table.hpp"
#include "philosopher.hpp"
#include "curses_wrapper.hpp"

template <typename T>
void dine(T philospher_names_list)
{
	dpp::table table(philospher_names_list.size());

	std::vector<dpp::philosopher> philosphers;
	philosphers.reserve(philospher_names_list.size());
	for (size_t i = 0; i < philospher_names_list.size(); ++i)
	{
		philosphers.emplace_back(
			philospher_names_list[i], 
			table, 
			table.forks[i], 
			table.forks[(i + 1) % philospher_names_list.size()]);
	}

	table.ready = true;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	table.ready = false;
}

int main()
{
	std::array names = {
		 "filozof_0", "filozof_1", "filozof_2", "filozof_3", "filozof_4", 
		 "filozof_5", "filozof_6", "filozof_7", "filozof_8", "filozof_9" };
	//dine(names);

	auto window = dpp::curses_wrapper();
	auto text = {"XD", "XDDD", "XDDDDDDDDDDDD"};
	window.print_centered(text);
	window.print("ELO", 0, 0);
	getch();

	return 0;
}