#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_error.h>
#include "ImageStorage.hpp"
//#include "Popup.hpp"
#include "Burster.hpp"
#include <random>
#include <future>
#include <exception>
//#include "../shared/Settings.h"
//#include "../shared/Debug.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <queue>

#ifdef _WIN32
#include <windows.h>

extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Convert Windows command line arguments to C-style command line arguments
    int argc = __argc;
    char** argv = __argv;

    // Call the standard main function
    return main(argc, argv);
}
#endif
//void playGif(IMG_Animation* Gif, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface* screen, SDL_Window* window);

void cleanup(std::vector<Popup*> Popups) {
	for (auto it = Popups.begin(); it != Popups.end();) {
		delete* it;
		it = Popups.erase(it++);
	}
}


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	Settings* Sett = ReadSettings();
	std::cout << sizeof(*Sett) << std::endl;
	CreateLogFile();

	//B:\EdgewarePlusPlus-main\EdgeWare\resource\img
	//L:/Steam/userdata/86245047/760/remote/244850/screenshots
	ImageStorage IMGLib = ImageStorage(Sett->ImageFolderPath);
	LOG(INFO, Sett->LoggingStrenght , "Getting Images from : "+ Sett->ImageFolderPath);
	const Uint8* state = SDL_GetKeyboardState(NULL);
	struct timeb _start;
	ftime(&_start);
	long long start = _start.time * 1000 + _start.millitm;
	struct timeb now;

	struct timeb lastPop;
	lastPop.time = 0;
	lastPop.millitm = 0;

	bool measuretime = false;

	int test = 2;
	int c = 0;
	bool run = true;
	std::vector<Popup*> Popups;

	std::thread popupPrepper;

	if (test == 2) {
		std::queue<Burster> buff;
		std::vector<Burster> trash;
		buff.push(Burster(*Sett, &IMGLib));
		buff.push(Burster(*Sett, &IMGLib));
		while (true) {
			SDL_PumpEvents();
			ftime(&now);
			if ((now.time * 1000 + now.millitm) - (lastPop.time * 1000 + lastPop.millitm) > Sett->TimeBetweenPopups) {
				ftime(&lastPop);
				buff.push(Burster(*Sett, &IMGLib));
				buff.front().burst();
				trash.push_back(buff.front());
				buff.pop();
				if (trash.begin()->checkBurstDone() == true) {
					std::cout << "deleting previous BurstBffer" << std::endl;
					trash.begin()->~Burster();
				}
				std::cout << "---------------------" << std::endl;
			}
		}
	}
	return(0);
}



void playGif(IMG_Animation* Gif, SDL_Renderer * renderer, SDL_Texture *texture, SDL_Surface* screen,SDL_Window* window) {
	static int Current_image;
	int Gif_length = Gif->count;
	SDL_RenderClear(renderer);
	SDL_DestroyTexture(texture);
	if (Current_image >= Gif_length) {
		Current_image = 0;
	}
	texture = SDL_CreateTextureFromSurface(renderer, Gif->frames[Current_image++]);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

