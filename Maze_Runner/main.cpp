#include "Engine/Board.h"

extern std::vector<std::thread> threads;

int main() {

	Board gameBoard = Board(200,200,0,800,800);
	gameBoard.GameLoop();

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
	return 0;
}