#pragma once

#include <fstream>
#include <string>

extern std::ofstream Log;

enum HornySeverity {
	INFO,
	WARNING,
	HERROR,
	FATAL
};

void LOG(HornySeverity s, std::string message);