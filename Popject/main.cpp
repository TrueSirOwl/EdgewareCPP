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
	IMG_Init(IMG_INIT_PNG);

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

	if (test == 1) {

		std::cout << "creating window" << std::endl;

		SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
		if (window == NULL) {
			LOG(HERROR, 0, "Window Creation Failed. AbortingPopup");
			return(1);
		}

		//clickthrough deleted

		std::cout << "creating renderer" << std::endl;


		SDL_Renderer* PopupRenderer = SDL_CreateRenderer(window, -1 ,SDL_RENDERER_ACCELERATED);
		if (PopupRenderer == NULL) {
			LOG(HERROR, 0, "Renderer Creation Failed. AbortingPopup");
		}

		std::string ContentPath = IMGLib.getRandomImage();
		std::cout << ContentPath << std::endl;

		if (ContentPath.substr(ContentPath.find_last_of('.') + 1) == "gif") {
			IMG_Animation* Gif = IMG_LoadAnimation(ContentPath.c_str());
			int last_image = 0;
			ContentType Content = GIF;
		}

		std::cout << "loading image" << std::endl;


		SDL_Surface* imageSurface = IMG_Load(ContentPath.c_str());



		if (imageSurface == NULL) {
			LOG(WARNING, 0, "Loading " + ContentPath + " Failed: " + (std::string)SDL_GetError());
			exit(1);
		}



		double sourceSize, targetSize;

		std::random_device rd;
		std::default_random_engine randomizerEngine(rd());
		std::uniform_real_distribution<double> scaleFactor(1, 1);
		sourceSize = 1 + (std::max<int>(imageSurface->w, imageSurface->h) / std::min<int>(1920, 1080));
		//sourceSize = 1 + (std::max<int>(this->imageSurface->w, this->imageSurface->h) / std::min<int>(1920, 1080));

		targetSize = scaleFactor(randomizerEngine);
		int resizeFactor = targetSize / sourceSize;
		int targetw = static_cast<int>(imageSurface->w * resizeFactor);
		int targeth = static_cast<int>(imageSurface->h * resizeFactor);

		SDL_SetWindowSize(window, targetw, targeth);

		std::cout << "resizing to: " << targetw << " " << targeth << std::endl;



		std::random_device rud;
		std::default_random_engine randomizerEngineu(rud());
		std::uniform_int_distribution<int> WhereH(0, 1080- targeth);
		std::uniform_int_distribution<int> WhereW(0, 1920- targetw);
		//std::uniform_int_distribution<int> WhereH(0, 1080 - this->target.h);
		//std::uniform_int_distribution<int> WhereW(0, 1920 - this->target.w);
		int ImageLocX = WhereW(randomizerEngineu);
		int ImageLocY = WhereH(randomizerEngineu);
		SDL_SetWindowPosition(window, ImageLocX, ImageLocY);

		std::cout << "placing at: " << ImageLocX << " " << ImageLocY << std::endl;


		SDL_SetWindowOpacity(window, 0.5);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
		std::cout << "creating texture" << std::endl;

		SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(PopupRenderer,imageSurface);
		if (imageTexture == NULL) {
			LOG(HERROR, 0 ,SDL_GetError());
			SDL_ClearError();
		}
		int renderError = SDL_RenderClear(PopupRenderer);
		if (renderError != 0) {
			LOG(HERROR, 0 ,SDL_GetError());
			SDL_ClearError();
		}
		std::cout << "rendering" << std::endl;
		SDL_RenderCopy(PopupRenderer, imageTexture, NULL, NULL);
		SDL_RenderPresent(PopupRenderer);

		timeb start;
		ftime(&start);
		ftime(&now);
		while ((now.time * 1000 + now.millitm) - (start.time * 1000 + lastPop.millitm) < 10000) {
			ftime(&now);
		}
	}

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
					trash.erase(trash.begin());
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

