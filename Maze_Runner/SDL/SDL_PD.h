#pragma once
#include "../../../Libraries/SDL2-2.0.16/include/SDL.h"
#include "../Definitions/Image/Color.h"
#include "../Definitions/Image/BMP.h"
#include <stdio.h>

class SDL_PD {
public:
	SDL_PD(const int& height, const int& width);
	int Init();
	void Cleanup();
	void SetDrawColor(Color color);
	void DrawPoint(const int& y, const int& x);
	void Clear();
	void DrawBMP(BMP image);

private:
	int m_height;
	int m_width;
	SDL_Window* window;
	SDL_Renderer* renderer;

};

