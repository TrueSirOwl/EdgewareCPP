#pragma once
#include "ImageStorage.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include <sys/timeb.h>
#include <thread>
#include <Settings.h>

enum ContentType
{
	IMAGE,
	GIF
};

typedef struct displayedImage {
	SDL_Rect pos;
	ContentType type;
	SDL_Texture* image = NULL;
	IMG_Animation* gif = NULL;
}slut;

class Popup
{
private:
	Settings sett;
	ImageStorage ImageLib;
	SDL_Renderer* PopupRenderer;
	SDL_Window* window;
	/*
	int displayNumber;
	*/
	std::vector<SDL_Rect>displaySizes;
	
	//SDL_Texture* imageTexture;
	//int imageW;
	//int imageH;
	//IMG_Animation *Gif;
	//int ImageLocX, ImageLocY;

	int Current_image;
	long long last_image;

	std::string ContentPath;


	double resizeFactor;
	SDL_Rect target;

	ContentType Content;

	struct timeb birth;
	bool born;

	slut outpop;


	void getImage();
	void scaleImage();
	//void getDisplays();
	void renderImage();
	void renderGif();
	void placer();
	void ImageThread();
	void GIFThread();
	void infinityGIFThread();
	void FadeOut();
	void GifFadeout();

	void SetWindowClickThrough();

public:
	long long lifetime;
	bool death;

	ContentType getContentType();
	Popup(ImageStorage& src, const Settings popsett, SDL_Renderer* PopupRenderer, SDL_Window* window, std::vector<SDL_Rect> displaySizes);
	
	Popup(ImageStorage& src, const Settings popsett, std::vector<SDL_Rect> displaySizes, SDL_Renderer* PopupRenderer);
	
	const slut GetOutpop();

	void PopUp();

	~Popup();

};

