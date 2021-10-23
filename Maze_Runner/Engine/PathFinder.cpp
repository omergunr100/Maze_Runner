#include "PathFinder.h"

int PathFinder::unique = 0;
int PathFinder::counter = 0;
std::shared_ptr<PathFinder> PathFinder::WallPointer = std::make_shared<PathFinder>(PathFinder::Wall, Vector2(-1, -1));


Color & PathFinder::GetColor(std::shared_ptr<PathFinder>& exColor)
{
	static Color Start = { 77, 137, 99 };
	static Color End = { 238, 50, 51 };
	static Color Runner = { 102,167,197 };
	static Color Trail = { 41, 64, 82 };
	static Color ChosenTrail = { 204, 255, 0 };
	static Color Else = { 1,1,1 };

	if (exColor->m_type == Type::Start)
		return Start;
	else if (exColor->m_type == Type::End)
		return End;
	else if (exColor->m_type == Type::Runner)
		return Runner;
	else if (exColor->m_type == Type::Trail)
		return Trail;
	else if (exColor->m_type == Type::ChosenTrail)
		return ChosenTrail;
	return Else;
}

PathFinder::PathFinder(Type type, const Vector2& loc)
{
	m_type = type;
	m_loc = loc;
	m_steps = 0;
	m_unique = ++unique;
	m_surrounded = false;
}

PathFinder::PathFinder(const PathFinder& other)
{
	m_type = other.m_type;
	m_loc = other.m_loc;
	m_unique = ++unique;
	m_steps = other.m_steps;
	m_neighbors = other.m_neighbors;
	m_surrounded = other.m_surrounded;
}

void PathFinder::Replace(const PathFinder & other)
{
	m_type = other.m_type;
	m_loc = other.m_loc;
	m_unique = ++unique;
	m_steps = other.m_steps;
	m_surrounded = other.m_surrounded;
}

void PathFinder::SetNeighbors(const std::vector<std::shared_ptr<PathFinder>>& neighbors)
{
	m_neighbors = neighbors;
}

int PathFinder::CalculateSteps()
{
	m_steps = INT_MAX;
	for (int i = 0; i < m_neighbors.size(); i++) {
		if(m_neighbors[i]->m_type != Wall && m_neighbors[i]->m_unique != m_unique)
			m_steps = m_steps > m_neighbors[i]->m_steps + 1 ? m_neighbors[i]->m_steps + 1 : m_steps;
	}
	return m_steps;
}

std::shared_ptr<PathFinder> PathFinder::FindShortestRoute()
{
	std::shared_ptr<PathFinder> shortest = nullptr;
	for (int i = 0; i < m_neighbors.size(); i++) {
		if (m_neighbors[i]->m_unique == m_unique)
			continue;
		if (m_neighbors[i]->m_type == Trail) {
			if (shortest == nullptr && m_neighbors[i]->m_type != Wall) {
				shortest = m_neighbors[i];
			}
			else if (shortest != nullptr && m_neighbors[i]->m_type != Wall) {
				if (shortest->m_steps > m_neighbors[i]->m_steps)
					shortest = m_neighbors[i];
			}
		}
	}
	return shortest;
}

void PathFinder::PlayTurn()
{
	
}

Vector2::Vector2()
{
	m_y = 0;
	m_x = 0;
}

Vector2::Vector2(int y, int x)
{
	m_y = y;
	m_x = x;
}
