#include "Burster.h"


Burster::Burster(Settings sett , ImageStorage& src): IMGLib(src) {
	for (int i = 0; i < sett.BurstAmt; i++)
	{
		Popup* pop = new Popup(IMGLib, sett);
		burstBuffer.push_back(pop);
	}
}

void Burster::burst() {
	std::vector<Popup*>::iterator it = burstBuffer.begin();
	while (it != burstBuffer.end()) {
		(*it)->PopUp();
		++it;
	}
}

Burster::~Burster() {
}