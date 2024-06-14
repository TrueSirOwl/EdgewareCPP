#pragma once
#include "Popup.hpp"
#include <vector>
#include <Settings.h>


#include <SDL.h>
#include <SDL_image.h>

class Burster
{
private:
	Settings sett;
	
	std::vector<slut> burstBuffer;
	ImageStorage IMGLib;

	std::vector<std::pair<SDL_Texture*,IMG_Animation*>>popBuffer;

	SDL_Renderer* PopupRenderer;
	SDL_Window* window;

	int displayNumber;
	std::vector<SDL_Rect> displaySizes;

	void getDisplays();
	void SetWindowClickThrough();

public:
	Burster(Settings sett, ImageStorage& src);
	void burst();
	~Burster();
};

