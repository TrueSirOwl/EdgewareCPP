#include "Popup.hpp"
#include <random>
#include <algorithm>
#include <Windows.h>
#include <chrono>

Popup::Popup(ImageStorage& src, const Settings popsett): sett(popsett), ImageLib(src), lifetime(sett.PopupLifespan), death(false), born(false){
	getDisplays();
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (this->window == NULL) {
		delete (this);
	}
	this->PopupRenderer = SDL_CreateRenderer(this->window, 2 ,SDL_RENDERER_ACCELERATED);
	if (this->PopupRenderer == NULL) {
		delete(this);
	}
	getImage();
	scaleImage();
	placer();
	SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	if (this->Content == GIF) {
		Gif = IMG_LoadAnimation(this->ContentPath.c_str());
	}
}

void Popup::getDisplays() {
	this->displayNumber = SDL_GetNumVideoDisplays();
	for ( int i = 0; i < this->displayNumber; i++) {
		this->displaySizes.push_back(SDL_Rect());
		SDL_GetDisplayBounds(i, &this->displaySizes.back());
	}

}

void Popup::getImage() {
	this->ContentPath = ImageLib.getRandomImage();
	std::cout << this->ContentPath << std::endl;
	/*
	while (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) != "gif") {
		this->ContentPath = ImageLib.getRandomImage();
		std::cout << this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) << std::endl;
	}
	*/
	this->imageTexture = IMG_LoadTexture(this->PopupRenderer, this->ContentPath.c_str());
	std::cout << "image error message: " << SDL_GetError() << std::endl;
	SDL_ClearError();
	SDL_QueryTexture(this->imageTexture, NULL, NULL, &this->imageW, &this->imageH);
	if (this->imageTexture == NULL) {
		delete(this);
	}
	this->Content = getContentType();
}

ContentType Popup::getContentType() {
	if (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) == "gif") {
		return(GIF);
	}
	else {
		return (IMAGE);
	}
}

void Popup::scaleImage() {
	
	double sourceSize, targetSize;

	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_real_distribution<double> scaleFactor(0.7, 1);
	sourceSize = 1 + (std::max<int>(this->imageW, this->imageH) / std::min<int>(this->displaySizes[0].w, this->displaySizes[0].h));
	targetSize = scaleFactor(randomizerEngine);
	this->resizeFactor = targetSize / sourceSize;
	this->target.w = static_cast<int>(this->imageW * resizeFactor);
	this->target.h = static_cast<int>(this->imageH * resizeFactor);

	SDL_SetWindowSize(this->window, this->target.w, this->target.h);

	/* get resize factor :
	source size is: max image dimension / min screen dimension
	target size is: randomized number (0-1)
	resize factor then is: target size / source size

	scale image:
	image height * resize factor
	image width * resize factor */
}

void Popup::placer() {
	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_int_distribution<int> WhereH(0, this->displaySizes[0].h - this->target.h);
	std::uniform_int_distribution<int> WhereW(0, this->displaySizes[0].w - this->target.w);
	this->ImageLocX = WhereW(randomizerEngine);
	this->ImageLocY = WhereH(randomizerEngine);
	SDL_SetWindowPosition(this->window, this->ImageLocX, this->ImageLocY);
}

void Popup::PopUp() {
	std::thread run;

	switch (this->Content) {
	case (GIF):
		renderGif();
		if (this->sett.PopupLifespan >= 0) {
			run = std::thread(&Popup::GIFThread, this);
			run.detach();
		}
		else {
			run = std::thread(&Popup::infinityGIFThread, this);
			run.detach();
		}
		break;
	case (IMAGE):
		renderImage();
		if (this->sett.PopupLifespan >= 0) {
			run = std::thread(&Popup::ImageThread, this);
			run.detach();
		}
		break;
	default:
		break;
	}
	SDL_ShowWindow(this->window);
	SDL_RenderPresent(this->PopupRenderer);
}

void Popup::GIFThread() {
	struct timeb start;
	ftime(&start);
	struct timeb now;
	ftime(&now);
	while (((long long)now.time * 1000 + now.millitm) - ((long long)start.time * 1000 + start.millitm) < this->lifetime) {
		ftime(&now);
		renderGif();
	}
	GifFadeout();
	delete(this);
}

void Popup::infinityGIFThread() {
	struct timeb start;
	ftime(&start);
	struct timeb now;
	ftime(&now);
	while (true) {
		ftime(&now);
		renderGif();
	}
	delete(this);
}

void Popup::ImageThread() {
	std::this_thread::sleep_for(std::chrono::milliseconds(this->lifetime));
	//std::cout << "age kill" << std::endl;
	if (this->sett.PopupFadeOut == true) {
		FadeOut();
	}
	delete(this);
}


void Popup::renderGif() {
	struct timeb now;
	ftime(&now);
	int Gif_length = Gif->count;
	if (this->Current_image >= Gif_length) {
		this->Current_image = 0;
	}
	if (((long long)now.time * 1000 + now.millitm) - last_image >= Gif->delays[this->Current_image]) {
		SDL_DestroyTexture(this->imageTexture);
		this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer, Gif->frames[this->Current_image++]);
		SDL_RenderCopy(this->PopupRenderer, this->imageTexture, NULL, NULL);
		SDL_RenderPresent(this->PopupRenderer);
		last_image = (long long)now.time * 1000 + now.millitm;
	}
}

void Popup::renderImage() {
	//SDL_DestroyTexture(this->imageTexture);
	//this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer,this->imageSurface);
	//SDL_FreeSurface(this->imageSurface);
	SDL_RenderCopy(this->PopupRenderer, this->imageTexture, NULL, NULL);
}

void Popup::GifFadeout() {
	double dimin = this->sett.PopupOpacity / this->sett.PopupFadeOutSteps;
	double step = (this->sett.PopupFadeOutTime / this->sett.PopupFadeOutSteps);

	struct timeb middle1;
	struct timeb middle;

	ftime(&middle);
	ftime(&middle1);

	while (this->sett.PopupOpacity > 0) {
		if ((middle.time * 1000 + middle.millitm) - (middle1.time * 1000 + middle1.millitm) >= step) {
			ftime(&middle1);
			SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
			renderGif();
			this->sett.PopupOpacity -= dimin;
		}
		ftime(&middle);
	}
}

void Popup::FadeOut() {
	double dimin = this->sett.PopupOpacity / this->sett.PopupFadeOutSteps;
	double step = (this->sett.PopupFadeOutTime / this->sett.PopupFadeOutSteps);

	struct timeb middle1;
	struct timeb middle;

	ftime(&middle);
	ftime(&middle1);

	while (this->sett.PopupOpacity > 0) {
		if ((middle.time * 1000 + middle.millitm) - (middle1.time * 1000 + middle1.millitm) >= step) {
			ftime(&middle1);
			SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
			this->sett.PopupOpacity -= dimin;
		}
		ftime(&middle);
	}
}

Popup::~Popup() {
	//std::cout << "Popup vaporised" << std::endl;
	SDL_HideWindow(this->window);
	if (this->PopupRenderer != NULL) {
		SDL_DestroyRenderer(this->PopupRenderer);
	}
	if (this->window != NULL) {
		SDL_DestroyWindow(this->window);
	}
}
