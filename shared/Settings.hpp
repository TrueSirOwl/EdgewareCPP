#pragma once

#include <string>
#include <map>
#include "Debug.hpp"

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
	int BurstAmt;
	bool PopupFadeOut;
	double PopupFadeOutSteps;
	int PopupFadeOutTime;
	double PopupOpacity;
	int LoggingStrenght;
	int Overlay;
};

Settings*	ReadSettings();
Setting		OwOWhatSettingDis(const std::string line, const std::map<std::string, Setting> lineToEnumMap);
void		assign(std::string line, Setting sett, Settings *settingStruct);
void		setStandardSettingsFile(Settings* sett);
