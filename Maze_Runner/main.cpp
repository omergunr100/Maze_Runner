#pragma once
#include "Engine/PathFinder.h"
#include "Engine/GameBoard.h"
#include <stdio.h>

//#include <chrono>

int main(int argc, char* argv[]) {

	// New engine
	std::shared_ptr<SDL_PD> window = std::make_shared<SDL_PD>(1080, 1920);
	window->Init();
	GameBoard board = GameBoard(window, 180, 160, 720, 1280, 4000);
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
	std::cin;
	window->Cleanup();
	return 0;
}