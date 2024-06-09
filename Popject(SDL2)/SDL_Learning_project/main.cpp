#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_error.h>
#include "ImageStorage.hpp"
#include "Popup.hpp"
#include "Burster.h"
#include <random>
#include <vector>
#include <future>
#include <exception>
#include <Settings.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <Debug.h>

std::ofstream Log;

void playGif(IMG_Animation* Gif, SDL_Renderer* renderer, SDL_Texture* texture, SDL_Surface* screen, SDL_Window* window);

void cleanup(std::vector<Popup*> Popups) {
	for (auto it = Popups.begin(); it != Popups.end();) {
		delete* it;
		it = Popups.erase(it++);
	}
}

void CreateLogFile() {
	auto runt = std::chrono::system_clock::now();
	std::time_t runTime = std::chrono::system_clock::to_time_t(runt);
	std::string time = "L:/CPP_learning_stuff/Logs/Log_";
	time.append(std::ctime(&runTime));
	time.erase(time.find("\n"));
	time.append(".hornylog");
	std::replace(time.begin(), time.end(), ' ', '_');
	std::replace(time.begin() + 2, time.end(), ':', '_');
	Log = std::ofstream(time.c_str());
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
	ftime(&now);
	if (measuretime == true) {
		std::cout << "start: " << (now.time * 1000 + now.millitm) - start << std::endl;
	}
	if (test == 1) {
		try
		{
			while (run == true) {
				ftime(&now);
				if ((now.time * 1000 + now.millitm) - (lastPop.time * 1000 + lastPop.millitm) > Sett->TimeBetweenPopups) {
					std::cout << (now.time * 1000 + now.millitm) - (lastPop.time * 1000 + lastPop.millitm) << " ms since last pop" << std::endl;
					ftime(&lastPop);
					Popup* pop = new Popup(IMGLib, *Sett);
					pop->PopUp();
					++c;
					if (measuretime == true) {
						ftime(&now);
						std::cout << "loading popup(" << c << ") took: " << (now.time * 1000 + now.millitm) - start << " ms" << std::endl;
						start = now.time * 1000 + now.millitm;
					}
				}
				SDL_PumpEvents();
				if (state[SDL_SCANCODE_E]) {
					exit(1);
				}
				if (measuretime == true) {
					ftime(&now);
					std::cout << "checking for input : " << (now.time * 1000 + now.millitm) - start << " ms" << std::endl;
					start = now.time * 1000 + now.millitm;
				}
				//updeath(Popups);
				if (measuretime == true) {
					ftime(&now);
					std::cout << "rendering and checking for death : " << (now.time * 1000 + now.millitm) - start << " ms" << std::endl;
					start = now.time * 1000 + now.millitm;
					std::cout << "===============loop completes===============" << std::endl;
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cout << typeid(e).name() << std::endl;
			std::cout << e.what() << std::endl;
		}
	}
	else if (test == 2) {
		std::vector<Burster*> buff;
		Burster* burst = new Burster(*Sett, IMGLib);
		buff.push_back(burst);
		burst = new Burster(*Sett, IMGLib);
		buff.push_back(burst);
		while (true) {
			SDL_PumpEvents();
			ftime(&now);
			if ((now.time * 1000 + now.millitm) - (lastPop.time * 1000 + lastPop.millitm) > Sett->TimeBetweenPopups) {
				ftime(&lastPop);
				Burster* burst = new Burster(*Sett, IMGLib);
				buff.push_back(burst);
				buff[1]->burst();
				std::cout << "deleting previous BurstBffer" << std::endl;
				buff.erase(buff.begin());
				std::cout << "---------------------" << std::endl;
			}
		}

	}
	else {
		if (IMG_Init(IMG_INIT_JPG & IMG_INIT_PNG & IMG_INIT_WEBP & IMG_INIT_AVIF & IMG_INIT_JXL) == 0) {
			std::cout << "Image init failed" << std::endl;
		}
	

		SDL_Window* window = nullptr;
		SDL_Texture* texture = nullptr;
		SDL_Surface* screen = nullptr;
		SDL_Surface* image = nullptr;
		SDL_Renderer* renderer = nullptr;

		window = SDL_CreateWindow("test",500,500,500,500,SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
		//SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer);
		bool running = true;
		SDL_Event e;
		double alpha = 1;
	

		IMG_Animation* Gif = nullptr;

		screen = SDL_GetWindowSurface(window);

		std::string ImagePath = IMGLib.getRandomImage();
		image = IMG_Load(ImagePath.c_str());
		texture = SDL_CreateTextureFromSurface(renderer, image);
	
		std::string path = ImagePath;
		int change = 0;

		int Ww = 0;
		int Wh = 0;
		if (image == NULL) {
			std::cout << "IMGfail" << std::endl;
		}

		if (texture == NULL) {
			std::cout << "TEXfail" << std::endl;
		}
		while (running) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					running = false;
				}
				if (e.type == SDL_MOUSEWHEEL) {
					if (e.wheel.y < 0) {
						alpha -= 0.01;
					}
					if (e.wheel.y > 0) {
						alpha += 0.01;
					}
					if (alpha > 1) {
						alpha = 1;
					}
					if (alpha < 0.01) {
						alpha = 0.01;
					}
					std::cout << alpha << std::endl;
					change = 1;
					ImagePath = IMGLib.getRandomImage();
					path = ImagePath;
					while (path.substr(path.find_last_of('.') + 1) != "gif") {
						ImagePath = IMGLib.getRandomImage();
						path = ImagePath;
						std::cout << path.substr(path.find_last_of('.') + 1) << std::endl;
					}
					IMG_FreeAnimation(Gif);
					Gif = IMG_LoadAnimation(ImagePath.c_str());
					SDL_SetWindowOpacity(window, alpha);
				}
			}
			if (path.substr(path.find_last_of('.') + 1) == "gif") {
				SDL_GetWindowSize(window, &Ww, &Wh);
				if (Ww != Gif->w && Wh != Gif->h) {
					SDL_SetWindowSize(window, Gif->w, Gif->h);
					//std::cout << Gif->frames[0]->format->format << std::endl;
					std::cout << Gif->count << std::endl;
				}
				playGif(Gif, renderer, texture, screen, window);
			}
			else if (change == 1) {
				SDL_FreeSurface(image);
				image = IMG_Load(ImagePath.c_str());
				SDL_DestroyTexture(texture);
				texture = SDL_CreateTextureFromSurface(renderer, image);
				SDL_RenderCopy(renderer, texture, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
			change = 0;
		}

		SDL_DestroyWindow(window);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(image);
		IMG_Quit();
		std::cout << "bye bye" << std::endl;
		return(0);
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

