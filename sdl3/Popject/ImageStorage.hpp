#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>
#include <filesystem>

#include <vector>
#include <string>
#include <iostream>


class ImageStorage
{
private:
	std::vector<std::string> ImageLib;

	bool ValidateFile(std::string path);

public:
	std::string getRandomImage();
	ImageStorage(std::string ImageFolderPath);
};

