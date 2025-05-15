#pragma once

#include "ImageStorage.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <ctime>
#include <sys/timeb.h>
#include <thread>
#include "../shared/Settings.hpp"
#include <thread>
#include <mutex>
//#include "../shared/Debug.h"

enum ContentType
{
	IMAGE,
	GIF
};

class Popup
{
private:
	Settings sett;

	ImageStorage ImageLib;
	SDL_Renderer* PopupRenderer;
	SDL_Window* window;
	SDL_Texture* imageTexture;
	SDL_Surface* imageSurface;

	int allowed_disps[3];

	SDL_FRect target;

	SDL_Rect* dispbounds;


	SDL_Thread* sdl_loader;

	IMG_Animation *Gif;
	int Current_image;
	long long last_image;

	std::string ContentPath;

	double resizeFactor;

	ContentType Content;

	struct timeb start;
	//struct timeb middle;


	void getImage();
	void scale();
	void place();
	static int getImageT(void* data);
	void renderImage();
	void renderGif();
	void DoImage();
	void DoGIF();
	void FadeOut();
	void GifFadeout();

	void SetWindowClickThrough();

public:
	long long lifetime;

	std::mutex CheckDeath;
	bool death;

	Popup(ImageStorage& src, const Settings popsett, SDL_Window* window, SDL_Renderer* renderer);

	void PopUp();
	void Prep();
	//Popup(SDL_Surface image ,const Settings popsett);

	~Popup();

};

