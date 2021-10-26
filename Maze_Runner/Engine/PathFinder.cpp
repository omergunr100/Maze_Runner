#include "PathFinder.h"

int PathFinder::unique = 0;
int PathFinder::counter = 0;
std::shared_ptr<PathFinder> PathFinder::WallPointer = std::make_shared<PathFinder>(PathFinder::Wall, Vector2(-1, -1));


Color & PathFinder::GetColor(std::shared_ptr<PathFinder>& exColor)
{
	static Color Start = { 0, 204, 0 };
	static Color End = { 255, 0, 0 };
	static Color Runner = { 153,0,76};
	static Color Trail = { 0, 0, 204 };
	static Color ChosenTrail = { 255, 255, 0 };
	static Color Wall = { 128,128,128 };
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
	else if (exColor->m_type == Type::Wall)
		return Wall;
	return Else;
}

PathFinder::PathFinder(Type type, const Vector2& loc)
{
	m_type = type;
	m_loc = loc;
	m_steps = 0;
	m_unique = ++unique;
	m_surrounded = false;
	m_last = false;
	m_turnPlayed = 0;
	m_square = -1;
}

PathFinder::PathFinder(const PathFinder& other)
{
	m_type = other.m_type;
	m_loc = other.m_loc;
	m_unique = ++unique;
	m_steps = other.m_steps;
	m_neighbors = other.m_neighbors;
	m_surrounded = other.m_surrounded;
	m_last = other.m_last;
	m_turnPlayed = other.m_turnPlayed;
	m_square = other.m_square;
}

void PathFinder::Replace(std::shared_ptr<PathFinder> other)
{
	m_type = other->m_type;
	m_unique = ++unique;
	m_steps = other->m_steps+1;
	m_surrounded = other->m_surrounded;
	m_turnPlayed = other->m_turnPlayed+1;
	m_square = other->m_square;
}

void PathFinder::SetNeighbors(const std::vector<std::shared_ptr<PathFinder>>& neighbors)
{
	m_neighbors = neighbors;
}

int PathFinder::CalculateSteps()
{
	m_steps = INT_MAX;
	for (int i = 0; i < m_neighbors.size(); i++) {
		if(m_neighbors[i]->m_type != Wall && m_neighbors[i]->m_type != Empty && m_neighbors[i]->m_unique != m_unique)
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
		if (m_neighbors[i]->m_type == Start)
			m_neighbors[i]->m_surrounded = true;
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

void PathFinder::PlayTurn(const int& turn)
{
	if (m_turnPlayed >= turn)
		return;
	m_turnPlayed = turn;
	if (m_type == Type::Runner) {
		// New code
		int full = 0;
		// Smart re-checks
		for (int i = 0; i < m_neighbors.size(); i++){
			if (m_neighbors[i]->m_type != Empty)
				full++;
			if (m_neighbors[i]->m_type == Runner && m_neighbors[i]->m_steps + 1 < m_steps) {
				Replace(m_neighbors[i]);
				full = 0;
			}
			else if (m_neighbors[i]->m_type == Trail && m_neighbors[i]->m_steps + 1 < m_steps) {
				Replace(m_neighbors[i]);
				m_type == Runner;
				full = 0;
			}
		}
		
		for (int i = 0; i < m_neighbors.size(); i++) {
			if (m_neighbors[i]->m_unique == m_unique)
				continue;
			if (m_neighbors[i]->m_type == Type::Empty){
				m_neighbors[i]->Replace(std::make_shared<PathFinder>(*this));
				break;
			}
		}
		if(full == 9)
			m_type = Type::Trail;
	}
	else if (m_type == Type::Trail) {
		m_steps = CalculateSteps();
	}
}

void PathFinder::IsSurrounded()
{
	int counter = 0;
	for (int i = 0; i < m_neighbors.size(); i++)
		if (m_neighbors[i]->m_type != Empty)
			counter++;
	m_surrounded = m_neighbors.size() == counter ? true : false;
}

void PathFinder::RecursiveLastCalculation()
{
	if (m_type == Type::Start) {
		m_steps = 0;
		m_last = true;
	}
	if (!m_last) {
		CalculateSteps();
	}
	else {
		return;
	}
	for (int i = 0; i < m_neighbors.size(); i++) {
		if (m_neighbors[i]->m_type == Trail && m_neighbors[i]->m_unique != m_unique) {
			m_neighbors[i]->RecursiveLastCalculation();
			if(m_countCalc >= TIMES_CALC)
				m_neighbors[i]->m_last = true;
		}
	}
	m_countCalc++;
}

std::shared_ptr<PathFinder> PathFinder::GetOffsetNeighbor(int y, int x)
{
	std::shared_ptr<PathFinder> ret = m_neighbors[C];
	bool negX = false;
	bool negY = false;
	if (y < 0) {
		negY = true;
		y = -y;
	}
	if (x < 0) {
		negX = true;
		x = -x;
	}
		
	for (int dy = 0; dy < y; dy++) {
		if (negY) {
			ret = m_neighbors[N];
		}
		else {
			ret = m_neighbors[S];
		}

	}
	for (int dx = 0; dx < x; dx++) {
		if (negX) {
			ret = m_neighbors[W];
		}
		else {
			ret = ret->m_neighbors[E];
		}
	}
	return ret;
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

bool Vector2::operator!=(const Vector2 & other)
{
	if (m_x != other.m_x || m_y != other.m_y)
		return true;
	return false;
}
