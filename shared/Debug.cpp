#include <Debug.h>

void LOG(HornySeverity s, std::string message) {
	switch (s) {
	case INFO:
		Log << "INFO: " << message << std::endl;
		break;
	case WARNING:
		Log << "WARNING: " << message << std::endl;
		break;
	case HERROR:
		Log << "ERROR: " << message << std::endl;
		break;
	case FATAL:
		Log << "FATAL: " << message << std::endl;
		break;
	default:
		break;
	}

}