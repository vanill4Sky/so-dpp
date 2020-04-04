#include <iostream>
#include <ncurses.h>
#include <thread>
#include <array>

#include "fork.hpp"
#include "table.hpp"
#include "philosopher.hpp"

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

template <typename T>
struct vec2
{
	vec2() {}
	vec2(T x, T y): x{x}, y{y} {}
	T x, y;
};

class curses_wrapper 
{
public:
	curses_wrapper()
	{
		initscr();			
		raw();				
		keypad(stdscr, TRUE);
		noecho();
	}

	~curses_wrapper()
	{
		echo();
		cbreak();
		endwin();				
	}

	void update() const 
	{
		refresh();
	}

	vec2<int> get_size() const
	{
		return { stdscr->_maxx, stdscr->_maxy };
	}

	template<typename T>
	void print_centered(T text) const 
	{
		auto win_size = get_size();

		int row = win_size.y / 2 - text.size() / 2;
		for (const auto line: text) 
		{
			int col{ static_cast<int>( 
				win_size.x / 2 - line.size() / 2) };
			print(line, row, col);
			++row;
		}
		update();
	}

	void print(const std::string& text) const
	{
		printw(text.c_str());
	}

	void print(std::string text, vec2<int> pos) const
	{
		mvprintw(pos.x, pos.y, text.c_str());
	}

	void print(std::string text, int pos_x, int pos_y) const
	{
		mvprintw(pos_x, pos_y, text.c_str());
	}
};

int main()
{
	std::array names = {
		 "filozof_0", "filozof_1", "filozof_2", "filozof_3", "filozof_4", 
		 "filozof_5", "filozof_6", "filozof_7", "filozof_8", "filozof_9" };
	dine(names);

	return 0;
}