#include "Debug.hpp"
#include <algorithm>
#include <ctime>
#include <chrono>

std::ofstream Log;

void CreateLogFile() {
	auto runt = std::chrono::system_clock::now();
	std::time_t runTime = std::chrono::system_clock::to_time_t(runt);
	std::string time = "L:/CPP_learning_stuff/Logs/Log_";
	time.append(std::ctime(&runTime));
	time.erase(time.find("\n"));
	time.append(".hornylog");
	std::replace(time.begin(), time.end(), ' ', '_');
	std::replace(time.begin() + 2, time.end(), ':', '_');
	std::ofstream Log(time.c_str());
}

void LOG(HornySeverity s, int strength, std::string message) {
	switch (s) {
	case INFO:
		if (strength <= INFO) {
			Log << "INFO: " << message << std::endl;
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