#pragma once

#include "../Engine/PathFinder.h"

class Square {
public:
	static int unique;

	Square() = delete;
	Square(std::shared_ptr<PathFinder> TopLeft, const int& height, const int& width);
	Square(std::vector<std::shared_ptr<PathFinder>> container, const int& height, const int& width);
	Square(const Square& other) = delete;
	~Square();
private:
	int m_unique;
	Vector2 m_TopLeft;
	int m_height;
	int m_width;
	std::vector<std::shared_ptr<PathFinder>> m_container;
	
public:
	std::vector<std::shared_ptr<PathFinder>> FindRoute(const Vector2& start, const Vector2& end);
	std::shared_ptr<PathFinder> GetIndex(const Vector2& loc);
	void InitalizeRunners(const Vector2& start, const Vector2 end);
	void PlayTurn();
};