#pragma once
#include "Popup.hpp"
//#include <vector>
//#include "../shared/Settings.h"

class Burster
{
private:
	std::vector<Popup*> burstBuffer;
	ImageStorage IMGLib;

public:
	Burster(ImageStorage& src, const Settings sett, SDL_Rect* displays , SDL_Window* wind, SDL_Renderer* renderer);
	void burst();
	bool prep();
	bool checkBurstDone();
	~Burster();
};

