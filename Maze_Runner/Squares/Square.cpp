#include "Square.h"

int Square::unique = 0;

Square::Square(std::shared_ptr<PathFinder> TopLeft, const int & height, const int & width)
	:m_height(height),
	m_width(width)
{
	std::shared_ptr<PathFinder> current;
	m_unique = ++unique;
	m_TopLeft = TopLeft->m_loc;
	m_container.resize(height);
	for (int y = 0; y < height; y++) {
		m_container[y].reserve(width);
		for (int x = 0; x < width; x++) {
			current = TopLeft->GetOffsetNeighbor(y, x);
			current->m_square = m_unique;
			m_container[y].emplace_back(current);
		}
	}
}

Square::Square(std::vector<std::vector<std::shared_ptr<PathFinder>>> container, const int & height, const int & width)
{
	m_unique = ++unique;
	m_container = container;
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			GetIndex(Vector2(y, x))->m_square = m_unique;
		}
	}
	m_TopLeft = container[0][0]->m_loc;
	m_height = height;
	m_width = width;
}

Square::~Square()
{
}

Square::Square(const int & height, const int & width)
{
	m_height = height;
	m_width = width;
}

void Square::SetStart(const Vector2 & start)
{
	GetIndex(start)->m_type = PathFinder::Start;
}

void Square::SetEnd(const Vector2 & end)
{
	GetIndex(end)->m_type = PathFinder::End;
}

std::vector<std::shared_ptr<PathFinder>> Square::FindRoute(const Vector2 & start, const Vector2 & end)
{
	std::vector<std::shared_ptr<PathFinder>> retVect;
	std::shared_ptr<PathFinder> current = GetIndex(start);

	while (current->m_loc != end) {
		
		
	}
	return retVect;
}

std::shared_ptr<PathFinder> Square::GetIndex(const Vector2 & loc)
{
	return m_container[loc.m_y][loc.m_x];
}

void Square::InitalizeRunners(const Vector2 & start, const Vector2 end)
{
	std::shared_ptr<PathFinder> current;
	for (int dy = -1; dy < 2; dy++) {
		for (int dx = -1; dx < 2; dx++) {
			current = GetIndex(start)->GetOffsetNeighbor(dy, dx);
			if (current->m_loc.m_x >= m_TopLeft.m_x && current->m_loc.m_x < m_TopLeft.m_x + m_width && current->m_loc.m_y >= m_TopLeft.m_y && current->m_loc.m_y < m_TopLeft.m_y + m_height) {
				current->m_type = PathFinder::Runner;
			}
		}
	}
}

void Square::PlayTurn()
{

}
