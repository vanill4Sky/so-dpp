namespace utils
{

template <typename T>
struct vec2
{
	vec2() {}
	vec2(T x, T y): x{x}, y{y} {}
	T x, y;
};

}