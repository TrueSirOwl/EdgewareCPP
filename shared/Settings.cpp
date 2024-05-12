#include "Settings.h"
#include <fstream>
#include <iostream>

Settings *ReadSettings() {
	Settings* SettingsStruct = new Settings;
	std::string line;
	std::ifstream setting("L:/CPP_learning_stuff/shared/Settings.txt");
	int lineNbr(0);

	if (setting.is_open() == false) {
		std::cout << "No settings file found" << std::endl;
		exit(1);
	}

	while (std::getline(setting, line)) {
		assign(line, OwOWhatSettingDis(line), SettingsStruct);
	}
	return(SettingsStruct);
}

Setting OwOWhatSettingDis(std::string line) {
	const std::string LineSetting = line.substr(0, line.find('='));
	const std::map<std::string, Setting> lineToEnumMap = {
		{"ButtonX", Setting::ButtonX},
		{"ButtonY", Setting::ButtonY},
		{"ButtonText", Setting::ButtonText},
		{"PopupLifespan", Setting::PopupLifespan},
		{"TimeBetweenPopups", Setting::TimeBetweenPopups},
		{"ImageFolderPath", Setting::ImageFolderPath},
		{"MinXButtonHeight", Setting::MinXButtonHeight},
		{"MaxXButtonHeight", Setting::MaxXButtonHeight},
		{"MinYButtonHeight", Setting::MinYButtonHeight},
		{"MaxYButtonHeight", Setting::MaxYButtonHeight},
		{"Multiplicator", Setting::Multiplicator},
		{"PopupFadeOut", Setting::PopupFadeOut},
		{"PopupFadeOutSteps", Setting::PopupFadeOutSteps},
		{"PopupFadeOutTime", Setting::PopupFadeOutTime},
		{"PopupOpacity", Setting::PopupOpacity}
	};
	std::map<std::string,Setting>::const_iterator it = lineToEnumMap.find(LineSetting);
	if (it != lineToEnumMap.end()) {
		return (it->second);
	}
	else {
		std::cerr << "invalid setting: " << LineSetting << std::endl;
		return (nosett);
	}
}

void assign(std::string line, Setting sett, Settings* SettingsStruct) {
	switch (sett)
	{
	case Setting::ButtonX:
		SettingsStruct->ButtonX = std::stoi(line.substr(line.find('=')+1, line.length()));
		break;

	case Setting::ButtonY:
		SettingsStruct->ButtonY = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::ButtonText:
		SettingsStruct->ButtonText = line.substr(line.find('=') + 1, line.length());
		break;

	case Setting::PopupLifespan:
		SettingsStruct->PopupLifespan = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::TimeBetweenPopups:
		SettingsStruct->TimeBetweenPopups = std::stod(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::ImageFolderPath:
		SettingsStruct->ImageFolderPath = line.substr(line.find('=') + 1, line.length());
		break;

	case Setting::MinXButtonHeight:
		SettingsStruct->MinXButtonHeight = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::MaxXButtonHeight:
		SettingsStruct->MaxXButtonHeight = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::MinYButtonHeight:
		SettingsStruct->MinYButtonHeight = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;
	
	case Setting::MaxYButtonHeight:
		SettingsStruct->MaxYButtonHeight = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::Multiplicator:
		SettingsStruct->Multiplicator = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::PopupFadeOut:
		SettingsStruct->PopupFadeOut = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;
	
	case Setting::PopupFadeOutSteps:
		SettingsStruct->PopupFadeOutSteps = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;
	
	case Setting::PopupFadeOutTime:
		SettingsStruct->PopupFadeOutTime = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::PopupOpacity:
		SettingsStruct->PopupOpacity = std::stod(line.substr(line.find('=') + 1, line.length()));
		break;

	default:
		break;
	}
}