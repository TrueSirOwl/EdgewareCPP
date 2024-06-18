#include "Settings.hpp"
#include <fstream>
#include <iostream>

Settings *ReadSettings() {
	Settings* SettingsStruct = new Settings;
	std::string line;
	std::ifstream setting("shared/Settings.txt");
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
		{"BurstAmt", Setting::BurstAmt},
		{"PopupFadeOut", Setting::PopupFadeOut},
		{"PopupFadeOutSteps", Setting::PopupFadeOutSteps},
		{"PopupFadeOutTime", Setting::PopupFadeOutTime},
		{"PopupOpacity", Setting::PopupOpacity},
		{"PopupOverlay", Setting::Overlay},
		{"LoggingStrength", Setting::LoggingStrength},
	};
	int settingsDone = 0;

	if (setting.is_open() == 0) {
		std::cout << "No settings file found" << std::endl;
		std::ofstream creator("shared/Settings.txt");
	}
	while (std::getline(setting, line)) {
		assign(line, OwOWhatSettingDis(line, lineToEnumMap), SettingsStruct);
		++settingsDone;
	}

	if (settingsDone < lineToEnumMap.size()) {
		LOG(HERROR,1,"Settings File Corrupt, restoring to standard settings file");
		setStandardSettingsFile(SettingsStruct);
	}
	return(SettingsStruct);
}

Setting OwOWhatSettingDis(std::string line, const std::map<std::string, Setting> lineToEnumMap) {
	const std::string LineSetting = line.substr(0, line.find('='));
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
		SettingsStruct->ButtonX = std::stoi(line.substr(line.find('=') + 1, line.length()));
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

	case Setting::BurstAmt:
		SettingsStruct->BurstAmt = std::stoi(line.substr(line.find('=') + 1, line.length()));
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

	case Setting::Overlay:
		SettingsStruct->Overlay = std::stod(line.substr(line.find('=') + 1, line.length()));
		break;

	case Setting::LoggingStrength:
		SettingsStruct->LoggingStrenght = std::stoi(line.substr(line.find('=') + 1, line.length()));
		break;

	default:
		break;
	}
}

void setStandardSettingsFile(Settings* sett) {
	sett->BurstAmt = 1;
	sett->ButtonText = "test";
	sett->ButtonX = 10;
	sett->ButtonY = 10;
	sett->ImageFolderPath = "";
	sett->LoggingStrenght = 0;
	sett->MaxXButtonHeight = 100;
	sett->MaxYButtonHeight = 100;
	sett->MinXButtonHeight = 10;
	sett->MinYButtonHeight = 10;
	sett->Multiplicator = 0;
	sett->Overlay = 1;
	sett->PopupFadeOut = true;
	sett->PopupFadeOutSteps = 1000;
	sett->PopupFadeOutTime = 1000;
	sett->PopupLifespan = 10000;
	sett->PopupOpacity = 0.50;
	sett->TimeBetweenPopups = 10000;

// 	std::ofstream Settings("shared/Settings.txt");
// 	if (Settings.is_open() == false) {
// 		std::cout << "error could not open Settings File" << std::endl;
// 	}
// 	Settings << "ButtonX=" << "0" << std::endl;
// 	Settings << "ButtonY=" << "0" << std::endl;
// 	Settings << "ButtonText=" << "0" << std::endl;
// 	Settings << "PopupLifespan=" << "0" << std::endl;
// 	Settings << "ImageFolderPath=" << "0" << std::endl;
// 	Settings << "TimeBetweenPopups=" << "0" << std::endl;
// 	Settings << "MinXButtonHeight=" << "0" << std::endl;
// 	Settings << "MaxXButtonHeight=" << "0" << std::endl;
// 	Settings << "MinYButtonHeight=" << "0" << std::endl;
// 	Settings << "MaxYButtonHeight=" "0" << std::endl;
// 	Settings << "BurstAmt=" << "0" << std::endl;
// 	Settings << "PopupFadeOut=" << "0" << std::endl;
// 	Settings << "PopupFadeOutSteps=" << "0" << std::endl;
// 	Settings << "PopupFadeOutTime=" << "0" << std::endl;
// 	Settings << "PopupOpacity=" << "0" << std::endl;
// 	Settings << "PopupOverlay=" << "0" << std::endl;
// 	Settings << "LoggingStrength=" << "0" << std::endl;
}

//		{"Multiplicator", Setting::Multiplicator},
//		{"EnableBurst", Setting::EnableBurst},
