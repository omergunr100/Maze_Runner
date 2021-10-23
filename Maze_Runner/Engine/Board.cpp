#include "Board.h"
#include<iostream>
#include<sstream>

using namespace std;

extern std::vector<std::thread> threads = std::vector<std::thread>(100);


Board::Board(int cols, int rows, int endTurn, int scaleX, int scaleY)
	:m_cols(cols),
	m_rows(rows),
	m_endTurn(endTurn),
	m_scaleX(scaleX),
	m_scaleY(scaleY),
	m_turnNumber(0)
{
	m_gameBoard.resize(m_rows);
	for (int i = 0; i < m_rows; i++) {
		m_gameBoard[i].resize(m_cols);
		for (int j = 0; j < m_cols; j++) {
			m_gameBoard[i][j] = AntVariant();
		}
	}
	cout << "Generated empty " << m_gameBoard[0].size() << ", " << m_gameBoard.size() << " board" << endl;
	std::vector<Vector2> startEnd = GetNRandomPoints(2);
	
	m_gameBoard[startEnd[0].m_y][startEnd[0].m_x] = AntVariant(AntVariant::SpecialId::Start);
	m_gameBoard[startEnd[1].m_y][startEnd[1].m_x] = AntVariant(AntVariant::SpecialId::End);
	
	
	for (int r = -1; r < 2; r++) {
		for (int c = -1; c < 2; c++) {
			if (startEnd[0].m_x + c < 0 || startEnd[0].m_x + c == m_cols || startEnd[0].m_y + r < 0 || startEnd[0].m_y + r == m_rows || (r == 0 && c == 0)) {
				continue;
			}
			else {
				m_gameBoard[startEnd[0].m_y + r][startEnd[0].m_x + c] = AntVariant::GetRunner();
			}
		}
	}
	

	cout << "Populated the board with start/end points" << endl;
	_mkdir(Config::GetAsChar("IMAGE_PATH"));
	TakeImage();
}

Board::Board(int cols, int rows, int endTurn)
	:m_cols(cols),
	m_rows(rows),
	m_endTurn(endTurn),
	m_scaleX(cols),
	m_scaleY(rows),
	m_turnNumber(0)
{
	m_gameBoard.resize(m_rows);
	for (int i = 0; i < m_rows; i++) {
		m_gameBoard[i].resize(m_cols);
		for (int j = 0; j < m_cols; j++) {
			m_gameBoard[i][j] = AntVariant();
		}
	}
	cout << "Generated empty " << m_gameBoard[0].size() << ", " << m_gameBoard.size() << " board" << endl;
	std::vector<Vector2> startEnd = GetNRandomPoints(2);
	m_gameBoard[startEnd[0].m_y][startEnd[0].m_x] = AntVariant(AntVariant::SpecialId::Start);
	m_gameBoard[startEnd[1].m_y][startEnd[1].m_x] = AntVariant(AntVariant::SpecialId::End);

	for (int r = -1; r < 2; r++) {
		for (int c = -1; c < 2; c++) {
			if (startEnd[0].m_x + c < 0 || startEnd[0].m_x + c == m_cols || startEnd[0].m_y + r < 0 || startEnd[0].m_y + r == m_rows) {
				continue;
			}
			else {
				m_gameBoard[startEnd[0].m_y + r][startEnd[0].m_x + c] = AntVariant::GetRunner();
			}
		}
	}

	cout << "Populated the board with start/end points" << endl;
	_mkdir(Config::GetAsChar("IMAGE_PATH"));
	TakeImage();
}

Board::~Board()
{
}


vector<Vector2> Board::GetNRandomPoints(const int & n)
{
	cout << "Started generating points (" << n << ")" << endl;
	vector<Vector2> resultVect(n);
	cout << "Generated vector of size: " << resultVect.size() << endl;
	for (int i = 0; i < n; i++) {
		int x = Randomness::dist_custom(0, m_cols-1), y = Randomness::dist_custom(0, m_rows-1);
		bool replicant = false;
		for (int j = 0; j < resultVect.size(); j++) {
			if (resultVect[j].m_x == x && resultVect[j].m_y == y){
				replicant = true;
				break;
			}
		}
		if(!replicant){
			cout << "Point " << i << ": [" << x << ", " << y << "]" << endl;
			resultVect[i] = Vector2(x, y);
		}
		else {
			i--;
			cout << "Duplicate, running again" << endl;
		}
	}
	return resultVect;
}


vector<BMP> Board::GetImages()
{
	return m_images;
}

vector<AntVariant> Board::GetImmediateNeighbors(const int & x, const int & y, vector<vector<AntVariant>>& changes)
{
	//cout << "Getting immediate neighbors" << endl;
	vector<AntVariant> neighbors;
	neighbors.reserve(9);
	for (int dy = -1; dy < 2; dy++) {
		for (int dx = -1; dx < 2; dx++) {
			if (x + dx < 0 || y + dy < 0 || x+dx >= m_cols || y+dy >= m_rows) {
				neighbors.push_back(AntVariant(AntVariant::SpecialId::PlaceHolder));
			}
			else {
				if (m_gameBoard[y + dy][x + dx].GetId() == changes[y + dy][x + dx].GetId()) {
					//cout << "Inserting neighbor from board: [" << x + dx << ", " << y + dy << "]" << endl;
					neighbors.push_back(m_gameBoard[y + dy][x + dx]);
				}
				else {
					//cout << "Inserting neighbor from changes: [" << x + dx << ", " << y + dy << "]" << endl;
					neighbors.push_back(changes[y + dy][x + dx]);
				}
			}
		}
	}
	return neighbors;
}

void Board::GameLoop()
{
	if (m_endTurn < 1) {
		cout << "Game-mode: Until all runners got to the finish line +" << -m_endTurn << endl;
		while (AntVariant::counter != m_rows * m_cols)
		{
			m_turnNumber++;
			cout << "Playing turn: " << m_turnNumber << endl;
			PlayTurn();
		}
		// Last one to clean last runners
		PlayTurn();
	}
	else {
		cout << "Game-mode: Until turn " << m_endTurn << endl;
		while (m_turnNumber <= m_endTurn)
		{
			m_turnNumber++;
			cout << "Playing turn: " << m_turnNumber << "; Turns remaining: " << m_endTurn - m_turnNumber << endl;
			PlayTurn();
		}
	}
	CreateTrail();
}

void Board::PlayTurn()
{
	vector<vector<AntVariant>> changes = m_gameBoard;
	//cout << "Runners left: " << AntVariant::toEnd << ", " << round(100 * AntVariant::toEnd / (m_rows*m_cols)) << "%" << endl;
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			if (m_gameBoard[y][x].GetId() != AntVariant::SpecialId::Runner)
				continue;
			//cout << "Playing: [" << x << ", " << y << "]" << endl;
			vector<AntVariant> results = m_gameBoard[y][x].PlayTurn(GetImmediateNeighbors(x,y,changes));
			int counter = 0;
			for (int dy = -1; dy < 2; dy++) {
				for (int dx = -1; dx < 2; dx++) {
					if(results[counter].GetId() != AntVariant::SpecialId::PlaceHolder)
						changes[y + dy][x + dx] = results[counter];						
					counter++;
				}
			}
		}
	}
	TakeImage();
	cout << endl;
	m_gameBoard = changes;
}

void Board::CreateTrail()
{
	Vector2 current;
	// Find end point
	for (int r = 0; r < m_rows; r++) {
		for (int c = 0; c < m_cols; c++) {
			if (m_gameBoard[r][c].GetId() == AntVariant::SpecialId::End) {
				current = Vector2(c, r);
			}
		}
	}
	// Find least steps path until start
	std::vector<AntVariant> results;
	while (m_gameBoard[current.m_y][current.m_x].GetId() != AntVariant::SpecialId::Start) {
		results = m_gameBoard[current.m_y][current.m_x].ChooseTrail(GetImmediateNeighbors(current.m_x, current.m_y, m_gameBoard));
		int counter = 0;
		bool stop = false;
		for (int dy = -1; dy < 2; dy++) {
			for (int dx = -1; dx < 2; dx++) {
				if (stop)
					break;
				if (results[counter].GetId() == AntVariant::SpecialId::ChosenTrail && results[counter].GetUnique() != m_gameBoard[current.m_y][current.m_x].GetUnique()) {
					m_gameBoard[current.m_y + dy][current.m_x + dx] = results[counter];
					current = Vector2(current.m_x + dx, current.m_y + dy);
					stop = true;
					m_gameBoard[current.m_y][current.m_x].SetId(AntVariant::SpecialId::UsedTrail);
				}
				else if (results[counter].GetId() == AntVariant::SpecialId::Start) {
					current = Vector2(current.m_x + dx, current.m_y + dy);
				}
				counter++;
			}
		}
		m_turnNumber++;
		TakeImage();
	}
	TakeImage();
}

BMP Board::TakeImage()
{
	cout << "Started taking image, turn: " << m_turnNumber << endl;
	BMP image = BMP(m_cols, m_rows);
	
	for (int y = 0; y < m_rows; y++) {
		for (int x = 0; x < m_cols; x++) {
			BMP::SetColor(image, m_gameBoard[y][x].GetColor(), x, y);
		}
	}
	stringstream ss;
	ss.str(string());
	ss << Config::GetAsChar("IMAGE_PATH") << "Image_" << m_turnNumber << ".bmp";
	std::string temp = ss.str();
	threads.emplace_back(&BMP::ThreadedExport, image, temp, m_scaleX, m_scaleY);
	/*
	if (m_cols == m_scaleX && m_rows == m_scaleY)
		BMP::Export(image, ss.str().c_str());
	else
		BMP::Export(image, ss.str().c_str(), m_scaleX, m_scaleY);
	*/
	//m_images.push_back(image);
	cout << "Finished exporting image, resolution: " << image.GetWidth() << ", " << image.GetHeight() << endl;
	return image;
}

Vector2::Vector2()
	:m_x(0),
	m_y(0)
{
}

Vector2::Vector2(int x, int y)
	:m_x(x),
	m_y(y)
{
}
