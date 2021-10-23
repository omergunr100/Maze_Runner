#pragma once

struct Color {
	int r, g, b;
	static const int size = 3;

	Color();
	Color(const Color& other);
	Color(const int& r, const int& g, const int& b);
	~Color();

	void operator+=(const Color& other);
	void operator-=(const Color& other);
};