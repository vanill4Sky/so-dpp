#include <iostream>
#include <ncurses.h>
#include <thread>
#include <array>

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
	curses_wrapper window;

	std::array<std::string, 7> text{ "Hello world!", "H", "e", "l", "l", "o", "!!!" };
	window.print_centered(text);
    getch();	

	return 0;
}