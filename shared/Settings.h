#pragma once

#include <string>
#include <map>
#include <Debug.h>

enum Setting
{
	nosett = -1,
	ButtonX,
	ButtonY,
	ButtonText,
	PopupLifespan,
	TimeBetweenPopups,
	ImageFolderPath,
	MinXButtonHeight,
	MaxXButtonHeight,
	MinYButtonHeight,
	MaxYButtonHeight,
	Multiplicator,
	EnableBurst,
	BurstAmt,
	PopupFadeOut,
	PopupFadeOutSteps,
	PopupFadeOutTime,
	PopupOpacity,
	LoggingStrength,
	Overlay,
};


struct Settings
{
	int ButtonX, ButtonY;
	std::string ButtonText;
	int PopupLifespan;
	double TimeBetweenPopups;
	std::string ImageFolderPath;
	int MinXButtonHeight;
	int MaxXButtonHeight;
	int MinYButtonHeight;
	int MaxYButtonHeight;
	int Multiplicator;
	int EnableBurst;
	int BurstAmt;
	bool PopupFadeOut;
	double PopupFadeOutSteps;
	int PopupFadeOutTime;
	double PopupOpacity;
	int LoggingStrenght;
	int Overlay;
};

Settings*	ReadSettings();
Setting		OwOWhatSettingDis(const std::string line);
void		assign(std::string line, Setting sett, Settings *settingStruct);
