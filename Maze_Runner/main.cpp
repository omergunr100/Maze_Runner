#pragma once
#include "Engine/PathFinder.h"
#include "Engine/GameBoard.h"
#include <stdio.h>

int main(int argc, char* argv[]) {

	// New engine
	std::shared_ptr<SDL_PD> window = std::make_shared<SDL_PD>(720, 1280);
	window->Init();
	GameBoard board = GameBoard(window, 360, 320, 720, 1280, 2000);
	board.Loop();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	window->Cleanup();
	return 0;
}