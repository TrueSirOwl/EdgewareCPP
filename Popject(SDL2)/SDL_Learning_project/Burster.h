#pragma once
#include "Popup.hpp"
#include <vector>
#include <Settings.h>

class Burster
{
private:
	std::vector<Popup*> burstBuffer;
	int amt;
	int lifetime;
	ImageStorage IMGLib;

public:
	Burster(Settings sett, ImageStorage& src);
	void burst();
};

