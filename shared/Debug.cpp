#include <Debug.h>

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