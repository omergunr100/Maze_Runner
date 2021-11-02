#pragma once
#include "PathFinder.h"
#include <sstream>
#include "../Definitions/Image/BMP.h"
#include "../SDL/SDL_PD.h"

class GameBoard {
public:
	GameBoard();
	GameBoard(std::shared_ptr<SDL_PD> window, const int& height, const int& width, const int& scaleHeight, const int& scaleWidth, const int& walls);
	~GameBoard();

private:
	int m_height;
	int m_width;
	int m_scaleHeight;
	int m_scaleWidth;
	int m_image;
	int m_turn;
	int m_walls;
	std::shared_ptr<PathFinder> m_start;
	std::shared_ptr<PathFinder> m_end;
	std::vector<std::vector<std::shared_ptr<PathFinder>>> m_gameBoard;
	std::shared_ptr<SDL_PD> m_window;

public:
	void Loop();

private:
	void InitializeBoard();
	void RecursiveLastCalculation(const Vector2& initial, const int& times);
	std::vector<Vector2> GetNRandomPoints(const int& n);
	BMP TakeImage();
};

