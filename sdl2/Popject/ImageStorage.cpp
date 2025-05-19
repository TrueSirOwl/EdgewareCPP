#include "ImageStorage.hpp"
#include <random>

std::string ImageStorage::getRandomImage() {
	if (this->ImageLib.empty()) {
		std::cerr << "No images found." << std::endl;
		return "";
	}
	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_int_distribution<int> random_number(0, RAND_MAX);
	int randomIndex = random_number(randomizerEngine) % this->ImageLib.size();
	return (this->ImageLib[randomIndex]);
}


ImageStorage::ImageStorage(std::string ImageFolderPath) {
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(ImageFolderPath)) {
		if (entry.is_regular_file() && this->ValidateFile(entry.path().string()) ) {
			this->ImageLib.push_back(entry.path().string());
		}
	}
}

bool ImageStorage::ValidateFile(std::string path) {
	if (path.substr(path.find_last_of('.') + 1) == "png" ||
		path.substr(path.find_last_of('.') + 1) == "jpg" ||
		path.substr(path.find_last_of('.') + 1) == "jpeg" || 
		path.substr(path.find_last_of('.') + 1) == "avif" ||
		path.substr(path.find_last_of('.') + 1) == "gif" ||
		path.substr(path.find_last_of('.') + 1) == "jxl") {
		//std::cout << path.substr(path.find_last_of('.') + 1, path.length()) << " added" << std::endl;
		return(true);
	}
	std::cout << path.substr(path.find_last_of('.') + 1, path.length()) << " rejected" << std::endl;
	return(false);
}