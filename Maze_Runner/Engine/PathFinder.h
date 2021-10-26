#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <iostream>
#include "../Definitions/Random/Randomness.h"
#include "../Definitions/Image/Color.h"
#include "../Definitions/Config/Config.h"

#define TIMES_CALC 30

struct Vector2 {
	int m_y, m_x;
	Vector2();
	Vector2(int y, int x);

	bool operator!=(const Vector2& other);
};

class PathFinder {
public:
	enum Type {
		Start, End, Runner, Trail, ChosenTrail, Wall, Empty
	};
	static int unique;
	static int counter;
	static std::shared_ptr<PathFinder> WallPointer;
	
	static Color& GetColor(std::shared_ptr<PathFinder>& exColor);

	PathFinder(Type type, const Vector2& loc);
	PathFinder(const PathFinder& other);

	void Replace(std::shared_ptr<PathFinder> other);
	void SetNeighbors(const std::vector<std::shared_ptr<PathFinder>>& neighbors);
	int CalculateSteps();
	std::shared_ptr<PathFinder> FindShortestRoute();
	void PlayTurn(const int& turn);
	void IsSurrounded();
	void RecursiveLastCalculation();
	std::shared_ptr<PathFinder> GetOffsetNeighbor(int y, int x);

private:
	enum Directions {
		NW, N, NE,
		W, C, E,
		SW, S, SE
	};

public:
	Type m_type;
	Vector2 m_loc;
	bool m_surrounded;
	int m_square;
private:
	int m_unique;
	int m_steps;
	std::vector<std::shared_ptr<PathFinder>> m_neighbors;
	bool m_last;
	int m_turnPlayed;
	int m_countCalc;
};
