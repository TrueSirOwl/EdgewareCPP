#include "Burster.hpp"

/*Creates an amount of Popups which starts the image loading thread for each Popup*/
Burster::Burster(ImageStorage& src, const Settings sett, SDL_Rect* displays , SDL_Window* wind, SDL_Renderer* renderer): IMGLib(src) {
	for (int i = 0; i < sett.BurstAmt; i++) {
		burstBuffer.push_back(new Popup(IMGLib, sett, displays, wind, renderer));
	}
}

void Burster::prep() {
	std::vector<Popup*>::iterator it = burstBuffer.begin();
	while (it != burstBuffer.end()) {
		(*it)->Prep();
		++it;
	}
}

void Burster::burst() {
	std::vector<Popup*>::iterator it = burstBuffer.begin();
	while (it != burstBuffer.end()) {
		(*it)->PopUp();
		++it;
	}
}

bool Burster::checkBurstDone() {
	std::vector<Popup*>::iterator it = burstBuffer.begin();
	while (it != burstBuffer.end()) {
		if ((*it)->death == true) {
			delete(*it);
			burstBuffer.erase(it);
		} else {
			++it;
		}
	}
	if (burstBuffer.empty() == true) {
		return (true);
	}
	return(false);
}

Burster::~Burster() {
	while (burstBuffer.empty() == false) {
		delete(burstBuffer.back());
		burstBuffer.pop_back();
	}
}