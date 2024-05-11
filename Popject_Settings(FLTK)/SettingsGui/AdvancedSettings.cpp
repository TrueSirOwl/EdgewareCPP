#include "AdvancedSettings.h"

AdvancedSettings::AdvancedSettings(int x, int y, int w, int h) : Fl_Window(x, y, w, h), X(x), Y(y), W(w), H(h) {
	box(FL_BORDER_BOX);

	this->SettingsFileContent = ReadSettings();

	this->MaxXButtonHeightInput = new Fl_Value_Input(50,0,70,20);
	this->MaxXButtonHeightInput->label("MaxX");
	this->MaxXButtonHeightInput->value(this->SettingsFileContent->MaxXButtonHeight);

	this->MinXButtonHeightInput = new Fl_Value_Input(50, 20, 70, 20);
	this->MinXButtonHeightInput->label("MinX");
	this->MinXButtonHeightInput->value(this->SettingsFileContent->MinXButtonHeight);

	this->MaxYButtonHeightInput = new Fl_Value_Input(200, 0, 70, 20);
	this->MaxYButtonHeightInput->label("MaxY");
	this->MaxYButtonHeightInput->value(this->SettingsFileContent->MaxYButtonHeight);

	this->MinYButtonHeightInput = new Fl_Value_Input(200, 20, 70, 20);
	this->MinYButtonHeightInput->label("MinY");
	this->MinYButtonHeightInput->value(this->SettingsFileContent->MinYButtonHeight);
	this->hide();
}
