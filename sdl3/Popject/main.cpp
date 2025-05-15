#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_blendmode.h>
#include "ImageStorage.hpp"
#include "Settings.hpp"
#include <unistd.h>
#include <string>
#include "Popup.hpp"
#include <signal.h>

SDL_HitTestResult MyHitTest(SDL_Window* win, const SDL_Point* area, void* data) {
	return SDL_HITTEST_NORMAL;
}

int main() {

	SDL_Init(SDL_INIT_VIDEO);

	Settings* Sett = ReadSettings();
	CreateLogFile();
	
	signal(SIGINT, SIG_DFL);
	//B:\EdgewarePlusPlus-main\EdgeWare\resource\img
	//L:/Steam/userdata/86245047/760/remote/244850/screenshots
	if (Sett->ImageFolderPath.empty() == true ) {
		std::cerr << "No Image path specified!" << std::endl;
		return(1);
	}
	ImageStorage IMGLib = ImageStorage(Sett->ImageFolderPath);
	LOG(INFO, Sett->LoggingStrenght , "Getting Images from : "+ Sett->ImageFolderPath);
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;
	
	bool succ = true;
	int dispnum;
	SDL_DisplayID* disps;
	std::vector<SDL_Rect> dispbounds;

	disps = SDL_GetDisplays(&dispnum);
	
	SDL_Rect dispb[dispnum];
	
	int c = 0;
	while(c < dispnum) {
		SDL_GetDisplayBounds(disps[c],&dispb[c]);
		dispbounds.push_back(dispb[c]);
		++c;
	}
	
	SDL_CreateWindowAndRenderer("title",dispbounds[0].w, dispbounds[0].h, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE| SDL_WINDOW_BORDERLESS,&window,&renderer);

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	std::vector<Popup*> pops;

	int i = 0;
	while (i < 10) {
		pops.push_back(new Popup(IMGLib, *Sett, window, renderer));
		++i;
	}

	std::vector<Popup*>::iterator it = pops.begin();

	while (it != pops.end()) {
		(*it)->Prep();
		++it;
	}
	it = pops.begin();
	while (true) {
		SDL_RenderClear(renderer);
		while (it != pops.end()) {
			(*it)->PopUp();
			if ((*it)->death == true) {
				delete(*it);
				pops.erase(it);
			} else {
				++it;
			}
		}
		SDL_RenderPresent(renderer);
		if (pops.size() < 10) {
			pops.push_back(new Popup(IMGLib, *Sett, window, renderer));
			pops.back()->Prep();
		}
		it = pops.begin();
	}

	sleep(2);
	
	it = pops.begin();
	while (it != pops.end()) {
		delete(*it);
		++it;
	}
	pops.clear();
	delete(Sett);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return (0);
}