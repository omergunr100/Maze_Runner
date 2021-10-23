#pragma once
#include <vector>
#include <direct.h>
#include "../Definitions/Config/Config.h"
#include "AntVariant.h"
#include "../Definitions/Image/BMP.h"

extern std::vector<std::thread> threads;

struct Vector2 {
	int m_x, m_y;
	Vector2();
	Vector2(int x, int y);
};

class Board {
private:
	int m_rows;
	int m_cols;
	int m_endTurn;
	int m_scaleX;
	int m_scaleY;
	int m_turnNumber;
	std::vector<std::vector<AntVariant>> m_gameBoard;
	std::vector<BMP> m_images;

public:
	Board(int cols, int rows, int endTurn, int scaleX, int scaleY);
	Board(int cols, int rows, int endTurn);
	~Board();
	
	std::vector<BMP> GetImages();
	void GameLoop();
private:
	std::vector<AntVariant> GetImmediateNeighbors(const int& x, const int& y, std::vector<std::vector<AntVariant>>& changes);
	std::vector<Vector2> GetNRandomPoints(const int & n);
	void PlayTurn();
	void CreateTrail();
	BMP TakeImage();
};