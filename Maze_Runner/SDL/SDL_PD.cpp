#include "SDL_PD.h"

SDL_PD::SDL_PD(const int & height, const int & width)
{
	m_height = height;
	m_width = width;
}

int SDL_PD::Init()
{
	window = NULL;
	renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL init, error: %d", SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow("Maze Runner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) {
		printf("SDL window, error: %d", SDL_GetError());
		return -2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return 0;
}

void SDL_PD::Cleanup()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void SDL_PD::SetDrawColor(Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
}

void SDL_PD::DrawPoint(const int & y, const int & x)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

void SDL_PD::Clear()
{
	SDL_RenderClear(renderer);
}

void SDL_PD::DrawBMP(BMP image)
{
	Clear();
	int scale_x, scale_y;
	scale_x = (int)floor(((double)m_width) / image.GetWidth());
	scale_y = (int)floor(((double)m_height) / image.GetHeight());
	int cur_x, cur_y;
	
	for (int y = 0; y < image.GetHeight(); y++) {
		for (int x = 0; x < image.GetWidth(); x++) {
			SetDrawColor(image.GetColor(x, y));
			for (int dy = 0; dy < (int)floor(scale_y); dy++) {
				for (int dx = 0; dx < (int)floor(scale_x); dx++) {
					DrawPoint(y*scale_y + dy, x*scale_x + dx);
				}
			}
		}
	}
	SDL_RenderPresent(renderer);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
