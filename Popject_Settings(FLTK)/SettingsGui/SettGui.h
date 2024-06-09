#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "PopupSettings.h"
#include "GeneralSettings.h"
#include "AdvancedSettings.h"

#include <FL/Fl_Box.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <vector>
#include <string>

enum ButtonPanel
{
	General,
	Popups,
	test1,
	test2
};

class SettGui
{
private:
	int W, H;

	int SelectorButtonW;
	int SelectorButtonH;

	Fl_Window* Gui;
	Fl_Button* saveButton;

	PopupSettings* PopSett;
	GeneralSettings* GenSett;
	AdvancedSettings* AdvSett;
	//int ButtonX;
	//int ButtonY;
	//char* ButtonText;
	//int PopupLifespan;

	std::vector<Fl_Button*> SelectorPanelButtons;
	std::vector<const char*> SelectorPanelButtonNames = {
	{"General"},
	{"Popups"},
	{"test1"},
	{"test2"},
	{"Advanced"}
	};

	static void ShowSettingWindow(Fl_Widget* w, void* name);

public:
	SettGui();
	void update(int CurrentlyOpenPageNum);

	void BuildMenueSelectorPanel(std::vector<const char*> &names);
	int GetCurrentlyOpenPage();

	void BuildPopupSettings();

	void setButtonX(int src);
	int getButtonX();
	void setButtonY(int src);
	int getButtonY();
	void setButtonText(char* src);
	const char* getButtonText();
	void setPopupLifespan(int src);
	int getPopupLifespan();
	void setImageFolderPath(char* src);
	const char* getImageFolderPath();
	double getTimeBetweenPopups();
	int getMinXButtonHeight();
	void setMinXButtonHeight(int src);
	int getMaxXButtonHeight();
	void setMaxXButtonHeight(int src);
	int getMinYButtonHeight();
	void setMinYButtonHeight(int src);
	int getMaxYButtonHeight();
	void setMaxYButtonHeight(int src);
	int getMultiplicatior();
	void setMultiplicatior(int src);
	int getPopupFadeOut();
	int getPopupFadeOutSteps();
	int getPopupFadeOutTime();
	double getPopupOpacity();
	int getPopupOverlay();
	int getLoggingStrength();
	int getBurstAmt();
};

void saveAndClose(Fl_Widget* Gui, void* Src);