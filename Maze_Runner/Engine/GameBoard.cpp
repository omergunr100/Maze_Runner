#include "GameBoard.h"

std::vector<std::thread> threads(100);

GameBoard::GameBoard()
{
}

GameBoard::GameBoard(SDL_PD* window, const int& height, const int& width, const int& scaleHeight, const int& scaleWidth, const int& walls) {
	m_height = height;
	m_width = width;
	m_scaleHeight = scaleHeight;
	m_scaleWidth = scaleWidth;
	m_image = 0;
	m_turn = 0;
	m_walls = walls;
	m_window = window;
	std::cout << "Initialized board stage 1" << std::endl;
	InitializeBoard();
}

GameBoard::~GameBoard()
{
}

void GameBoard::Loop()
{
	while (!m_end->m_surrounded) {
		TakeImage();
		for(int r = 0; r < m_height; r++)
			for (int c = 0; c < m_width; c++) {
				if (m_gameBoard[r][c]->m_type == PathFinder::Runner)
					m_gameBoard[r][c]->PlayTurn(m_turn);
			}
		m_end->IsSurrounded();
		m_turn++;
	}
	
	m_start->RecursiveLastCalculation();
	std::shared_ptr<PathFinder> trail = m_end->FindShortestRoute();
	while (!m_start->m_surrounded) {
		TakeImage();
		if (trail->m_type == PathFinder::Start)
			m_start->m_surrounded = true;
		if (!m_start->m_surrounded) {
			trail->m_type = PathFinder::ChosenTrail;
			trail = trail->FindShortestRoute();
		}		
	}
	TakeImage();

	// Taking care of the unfinished image saving threads
	for (int i = 0; i < threads.size(); i++)
		if (threads[i].joinable())
			threads[i].join();
}

void GameBoard::InitializeBoard() {
	// Resizing GameBoard and placing initial empty spots
	m_gameBoard.resize(m_height);		
	for (int r = 0; r < m_height; r++){
		m_gameBoard[r].resize(m_width);
		for (int c = 0; c < m_width; c++)
			m_gameBoard[r][c] = std::make_shared<PathFinder>(PathFinder::Type::Empty, Vector2(r,c));
	}
	std::cout << "Placed all initial points on the board" << std::endl;

	// Setting neighbors
	int counter;
	for(int y = 0; y < m_height; y++)
		for (int x = 0; x < m_width; x++) {
			std::vector<std::shared_ptr<PathFinder>> neighbors(9);
			counter = 0;
			for(int dy = -1; dy < 2; dy++)
				for (int dx = -1; dx < 2; dx++) {
					if (y + dy >= 0 && y + dy < m_height && x + dx >= 0 && x + dx < m_width) {
						neighbors[counter] = m_gameBoard[y + dy][x + dx];
					}
					else {
						neighbors[counter] = PathFinder::WallPointer;
					}
					counter++;
				}
			m_gameBoard[y][x]->SetNeighbors(neighbors);
		}
	std::cout << "Finished setting neighbors for all points on the board" << std::endl;

	// Placing start and end points
	std::vector<Vector2> startEnd = GetNRandomPoints(2+m_walls);
	m_start = m_gameBoard[startEnd[0].m_y][startEnd[0].m_x];
	m_start->m_type = PathFinder::Start;
	m_end = m_gameBoard[startEnd[1].m_y][startEnd[1].m_x];
	m_end->m_type = PathFinder::End;
	// surrounding start with runners
	for(int r = -1; r < 2; r++)
		for (int c = -1; c < 2; c++) {
			if (m_start->m_loc.m_y + r >= 0 && m_start->m_loc.m_y + r < m_height && m_start->m_loc.m_x + c >= 0 && m_start->m_loc.m_x + c < m_width && !(r == 0 && c == 0)) {
				m_gameBoard[m_start->m_loc.m_y + r][m_start->m_loc.m_x + c]->Replace(m_start);
				m_gameBoard[m_start->m_loc.m_y + r][m_start->m_loc.m_x + c]->m_type = PathFinder::Runner;
			}
		}
	if(m_walls > 0)
		for (int i = 2; i < startEnd.size(); i++)
			m_gameBoard[startEnd[i].m_y][startEnd[i].m_x]->m_type = PathFinder::Wall;
		
	std::cout << "Finished placing start/end/runners for start" << std::endl;
	std::cout << "Finished initialization of board stage 2" << std::endl;
}

std::vector<Vector2> GameBoard::GetNRandomPoints(const int & n)
{
	std::cout << "Started generating points (" << n << ")" << std::endl;
	std::vector<Vector2> resultVect(n);
	std::cout << "Generated vector of size: " << resultVect.size() << std::endl;
	for (int i = 0; i < n; i++) {
		int x = Randomness::dist_custom(0, m_width - 1), y = Randomness::dist_custom(0, m_height - 1);
		bool replicant = false;
		for (int j = 0; j < resultVect.size(); j++) {
			if (resultVect[j].m_x == x && resultVect[j].m_y == y) {
				replicant = true;
				break;
			}
		}
		if (!replicant) {
			std::cout << "Point " << i << ": [" << x << ", " << y << "]" << std::endl;
			resultVect[i] = Vector2(x, y);
		}
		else {
			i--;
			std::cout << "Duplicate, running again" << std::endl;
		}
	}
	return resultVect;
}

BMP GameBoard::TakeImage()
{
	BMP image = BMP(m_width, m_height);

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			BMP::SetColor(image,PathFinder::GetColor(m_gameBoard[y][x]), x, y);
		}
	}
	std::stringstream ss;
	ss.str(std::string());
	ss << Config::GetAsChar("IMAGE_PATH") << "Image_" << m_image << ".bmp";
	std::string temp = ss.str();
	threads.emplace_back(&BMP::ThreadedExport, image, temp, m_scaleWidth, m_scaleHeight);
	std::cout << "Finished exporting image, resolution: " << image.GetWidth() << ", " << image.GetHeight() << std::endl;
	m_image++;
	m_window->DrawBMP(image);
	return image;
}

