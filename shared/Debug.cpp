#include "Debug.hpp"
#include <algorithm>
#include <ctime>
#include <chrono>
#include <filesystem>

std::ofstream Log;

void CreateLogFile() {
	std::filesystem::create_directory("logs");

	auto runt = std::chrono::system_clock::now();
	std::time_t runTime = std::chrono::system_clock::to_time_t(runt);
	std::string time = "logs/Log_";
	time.append(std::ctime(&runTime));
	time.erase(time.find("\n"));
	time.append(".hornylog");
	std::replace(time.begin(), time.end(), ' ', '_');
	std::replace(time.begin() + 2, time.end(), ':', '_');
	Log = std::ofstream(time.c_str());
}

	#include <iostream>

void LOG(HornySeverity s, int strength, std::string message) {
	std::cout << "logging func called with: " << message << std::endl;
	switch (s) {
	case INFO:
		if (strength <= INFO) {
			Log << "INFO: " << message << std::endl;
			std::cout << "INFO: " << message << std::endl;
			Log.flush();
		}
		break;
	case WARNING:
		if (strength <= WARNING) {
			Log << "WARNING: " << message << std::endl;
		}
		break;
	case HERROR:
		if (strength <= HERROR) {
			Log << "ERROR: " << message << std::endl;
		}
		break;
	case FATAL:
		if (strength <= FATAL) {
			Log << "FATAL: " << message << std::endl;
		}
		break;
	default:
		break;
	}
}