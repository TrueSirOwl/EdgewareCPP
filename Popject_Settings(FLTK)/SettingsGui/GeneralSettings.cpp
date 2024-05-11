#pragma once

#include "GeneralSettings.h"

GeneralSettings::GeneralSettings(int x, int y, int w , int h): Fl_Window(x, y, w, h) {

	box(FL_BORDER_BOX);

	hide();
}