#pragma once
#include "PathFinder.h"
#include <sstream>
#include "../Definitions/Image/BMP.h"

extern std::vector<std::thread> threads(100);

class Board {
public:
	Board() = delete;
	Board(const int& height, const int& width, const int& scaleHeight, const int& scaleWidth);
	~Board();

private:
	int m_height;
	int m_width;
	int m_scaleHeight;
	int m_scaleWidth;
	int m_image;
	std::shared_ptr<PathFinder> m_start;
	std::shared_ptr<PathFinder> m_end;
	std::vector<std::vector<std::shared_ptr<PathFinder>>> m_gameBoard;


public:
	void Loop();

private:
	void InitializeBoard();
	std::vector<Vector2> GetNRandomPoints(const int& n);
	BMP TakeImage();
};

