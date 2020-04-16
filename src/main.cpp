#include <iostream>
#include <ncurses.h>
#include <thread>
#include <array>
#include <stdexcept>

#include "fork.hpp"
#include "table.hpp"
#include "philosopher.hpp"
#include "visualization.hpp"

template <typename T>
void dine(T philospher_names_list, int eating_duration, 
	int thinking_duration, int simulation_duration)
{
	dpp::table table(philospher_names_list.size());
	auto& visualization = dpp::visualization::getInstance();

	std::vector<dpp::philosopher> philosphers;
	philosphers.reserve(philospher_names_list.size());
	for (size_t i = 0; i < philospher_names_list.size(); ++i)
	{
		philosphers.emplace_back(
			i,
			philospher_names_list[i], 
			table, 
			table.forks[i], 
			table.forks[(i + 1) % philospher_names_list.size()],
			thinking_duration,
			eating_duration,
			visualization);
		visualization.add_philosopher(
			i,
			philospher_names_list[i]);
	}

	table.ready = true;
	std::this_thread::sleep_for(std::chrono::seconds(simulation_duration));
	table.ready = false;

	visualization.halt();
}

std::vector<std::string> philosophers_names(size_t N)
{
	std::vector<std::string> names;

	for(size_t i = 0; i < N; ++i)
	{
		names.emplace_back("philosopher_" + std::to_string(i));
	}

	return names;
}

int main(int argc, char* argv[])
{
	int philospohers_count{ 10 };
	int eating_duration{ 50 };
	int thinking_duration{ 100 };
	int simulation_duration{ 10 };

	for (int i = 1; i < argc; ++i)
	{
		std::string_view arg{ argv[i] };

		if(i + 1 >= argc)
		{
			break;
		}

		int arg_value;
		try 
		{
			arg_value = std::stoi(argv[++i]);
		}
		catch(const std::invalid_argument& e)
		{
			std::cerr << e.what() << '\n';
			break;
		}

		if (arg == "-pc")
		{
			philospohers_count = arg_value;
		}
		else if (arg == "-ed")
		{
			eating_duration = arg_value;
		}
		else if (arg == "-td")
		{
			thinking_duration = arg_value;
		}
		else if (arg == "-sd")
		{
			simulation_duration = arg_value;
		}
	}

	auto names = philosophers_names(philospohers_count);
	dine(names, eating_duration, thinking_duration, simulation_duration);

	return 0;
}