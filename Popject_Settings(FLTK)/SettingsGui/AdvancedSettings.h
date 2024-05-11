#pragma once
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Input.H>
#include "Settings.h"

class AdvancedSettings : public Fl_Window
{
private:

	Settings* SettingsFileContent;

public:

	int X, Y, W, H;

	Fl_Value_Input* MinXButtonHeightInput;
	Fl_Value_Input* MaxXButtonHeightInput;
	Fl_Value_Input* MinYButtonHeightInput;
	Fl_Value_Input* MaxYButtonHeightInput;

	AdvancedSettings(int x, int y, int w, int h);
};

