#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
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
	ImageStorage IMGLib = ImageStorage(Sett->ImageFolderPath);
	LOG(INFO, Sett->LoggingStrenght , "Getting Images from : "+ Sett->ImageFolderPath);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;

	bool succ = true;

	int dispnum;
	SDL_DisplayID* disps;
	
	disps = SDL_GetDisplays(&dispnum);
	
	SDL_Rect dispbounds[dispnum];

	int c = 0;
	while(c < dispnum) {
		SDL_GetDisplayBounds(disps[c],&dispbounds[c]);
		std::cout << "display " << c << " has a size of: " << dispbounds[c].h << ", " << dispbounds[c].w << std::endl;
		++c;
	}

	SDL_CreateWindowAndRenderer("title",dispbounds[0].w,dispbounds[0].h,SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE| SDL_WINDOW_BORDERLESS,&window,&renderer);


	std::string path = IMGLib.getRandomImage();
	surface = IMG_Load(path.c_str());
	std::cout << "image path: " << path << std::endl;
	if (surface == NULL) {
		std::cerr << "Image Loading failed!" << std::endl;
	}

	if (SDL_SetSurfaceAlphaMod(surface,	55) == false) {
		std::cout << "setting alpha failed" << std::endl;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_SetWindowShape(window,surface);
	SDL_FRect rec;
	rec.h=500;
	rec.w=500;
	rec.x=100;
	rec.y=100;

	SDL_RenderClear(renderer);

	std::vector<Popup*> pops;

	int i = 0;
	while (i < 50) {
		pops.push_back(new Popup(IMGLib, *Sett, window, renderer));
		++i;
	}

	std::vector<Popup*>::iterator it = pops.begin();

	while (it != pops.end()) {
		(*it)->PopUp();
		++it;
	}
	sleep(1);
	
	it = pops.begin();
	while (it != pops.end()) {
		delete(*it);
		++it;
	}
	pops.clear();
	delete(Sett);
	free(disps);
	SDL_DestroySurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return (0);
}