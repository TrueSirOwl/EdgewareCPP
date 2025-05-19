#include "GeneralSettings.hpp"

GeneralSettings::GeneralSettings(int x, int y, int w , int h, Settings* sett): Fl_Window(x, y, w, h), SettingsFileContent(sett){


	//this->SettingsFileContent = ReadSettings();
	box(FL_BORDER_BOX);


	hide();
}