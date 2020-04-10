#include <cmath>

namespace utils
{

template <typename T>
struct vec2
{
	vec2() {}
	vec2(T x, T y): x{x}, y{y} {}
	T x, y;
};

template <typename T>
T cartesian_to_polar(int x, int y)
{
	T phi;
	if (x > 0 && y >= 0)
		phi = std::atan(y / static_cast<T>(x));
	else if (x > 0 && y < 0)
		phi = std::atan(y / static_cast<T>(x)) + 2 * M_PI;
	else if (x < 0)
		phi = atan(y / static_cast<T>(x)) + M_PI;
	else if (x == 0 && y > 0)
		phi = M_PI / 2;
	else
		phi = 3 * M_PI / 2;

	return phi;
}

}