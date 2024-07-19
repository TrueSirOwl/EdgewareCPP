#include "Burster.hpp"

Burster::Burster(Settings sett , ImageStorage* src): IMGLib(src) {
	for (int i = 0; i < sett.BurstAmt; i++) {
		burstBuffer.push_back(new Popup(*IMGLib, sett));
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
		(*it)->CheckDeath.lock();
		if ((*it)->death == true) {
			(*it)->CheckDeath.unlock();
			delete(*it);
			burstBuffer.erase(it);
		} else {
			(*it)->CheckDeath.unlock();
			++it;
		}
	}
	if (burstBuffer.empty() == true) {
		return (true);
	}
	return(false);
}

Burster::~Burster() {
	burstBuffer.clear();
}