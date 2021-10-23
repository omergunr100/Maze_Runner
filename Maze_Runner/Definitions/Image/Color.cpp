#include "Color.h"

Color::Color()
	:r(0), g(0), b(0)
{
}

Color::Color(const Color & other)
	:r(other.r),
	g(other.g),
	b(other.b)
{
}

Color::Color(const int& r, const int& g, const int& b)
	: r(r), g(g), b(b)
{
}

Color::~Color()
{
}

void Color::operator+=(const Color & other)
{
	r += other.r;
	g += other.g;
	b += other.b;
}

void Color::operator-=(const Color & other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
}
