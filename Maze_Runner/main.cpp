#pragma once
#include "Engine/PathFinder.h"
#include "Engine/GameBoard.h"
#include <stdio.h>

//#include <chrono>

int main(int argc, char* argv[]) {

	// New engine
	SDL_PD* window = new SDL_PD(1000, 1000);
	window->Init();
	GameBoard board = GameBoard(window, 100, 100, 1000, 1000, 1500);
	board.Loop();

	// Old engine
	/*
	Board gameBoard = Board(200,200,0,800,800);
	gameBoard.GameLoop();

	for (int i = 0; i < threads.size(); i++) {
		if (threads[i].joinable())
			threads[i].join();
	}
	*/
	window->Cleanup();
	delete window;
	return 0;
}