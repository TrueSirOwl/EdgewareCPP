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

class Popup
{
private:
	Settings sett;

	ImageStorage ImageLib;
	SDL_Renderer* PopupRenderer;
	SDL_Window* window;
	SDL_Texture* imageTexture;
	
	int imageW;
	int imageH;

	IMG_Animation *Gif;
	int Current_image;
	long long last_image;

	std::string ContentPath;

	int displayNumber;
	std::vector<SDL_Rect>displaySizes;

	int ImageLocX, ImageLocY;

	double resizeFactor;
	SDL_Rect target;

	ContentType Content;

	struct timeb birth;
	bool born;


	void getImage();
	void scaleImage();
	void getDisplays();
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
	Popup(ImageStorage& src, const Settings popsett);

	void PopUp();

	~Popup();

};

