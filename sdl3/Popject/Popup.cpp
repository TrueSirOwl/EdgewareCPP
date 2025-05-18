#include "Popup.hpp"
#include <random>
#include <algorithm>
#include <chrono>
#include <SDL3/SDL_thread.h>

Popup::Popup(ImageStorage& src, const Settings popsett, SDL_Window* wind, SDL_Renderer* renderer): 
sett(popsett), ImageLib(src), lifetime(sett.PopupLifespan), death(false),
Current_image(0), imageSurface(NULL), imageTexture(NULL), Gif(NULL), Content(IMAGE), last_image(0), window(wind), PopupRenderer(renderer) {
	this->start = {};
	sdl_loader = SDL_CreateThreadRuntime(getImageT, "loader", this, NULL, NULL);
}

int Popup::getImageT(void* data) {
	Popup* dota = static_cast<Popup*>(data);
	dota->getImage();
}

void Popup::getImage() {
	this->ContentPath = ImageLib.getRandomImage();
	//std::cout << this->ContentPath << std::endl;

	if (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) == "gif") {
		this->Gif = IMG_LoadAnimation(this->ContentPath.c_str());
		this->Content = GIF;
		//std::cout << "loading gif" << std::endl;
	}
		this->imageSurface = IMG_Load(this->ContentPath.c_str());

	if (this->imageSurface == NULL) {
		LOG(WARNING, this->sett.LoggingStrenght, "Loading " + this->ContentPath + " Failed: " + (std::string)SDL_GetError());
		exit(1);
	}
}

void Popup::scale() {
	

	int dispnum;
	SDL_DisplayID* disps;
	
	disps = SDL_GetDisplays(&dispnum);
	
	SDL_Rect dispbounds[dispnum];

	int c = 0;
	while(c < dispnum) {
		SDL_GetDisplayBounds(disps[c],&dispbounds[c]);
		++c;
	}

	double sourceSize, targetSize;

	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_real_distribution<double> scaleFactor(this->sett.ImageSizeMin, this->sett.ImageSizeMax);
	sourceSize = 1 + (std::max<int>(this->imageSurface->w, this->imageSurface->h) / std::min<int>(dispbounds[0].w, dispbounds[0].h));
	//sourceSize = 1 + (std::max<int>(this->imageSurface->w, this->imageSurface->h) / std::min<int>(1920, 1080));

	targetSize = scaleFactor(randomizerEngine);
	this->resizeFactor = targetSize / sourceSize;
	this->target.w = static_cast<int>(this->imageSurface->w * resizeFactor);
	this->target.h = static_cast<int>(this->imageSurface->h * resizeFactor);
}

void Popup::place() {

	int dispnum;
	SDL_DisplayID* disps;
	
	disps = SDL_GetDisplays(&dispnum);
	
	SDL_Rect dispbounds[dispnum];

	int c = 0;
	while(c < dispnum) {
		SDL_GetDisplayBounds(disps[c],&dispbounds[c]);
		++c;
	}

	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_int_distribution<int> WhereH(0, dispbounds[0].h - this->target.h);
	std::uniform_int_distribution<int> WhereW(0, dispbounds[0].w - this->target.w);
	//std::uniform_int_distribution<int> WhereH(0, 1080 - this->target.h);
	//std::uniform_int_distribution<int> WhereW(0, 1920 - this->target.w);
	this->target.x = WhereW(randomizerEngine);
	this->target.y = WhereH(randomizerEngine);
}


void Popup::Prep() {
	if (SDL_GetThreadState(this->sdl_loader) == SDL_GetThreadState(this->sdl_loader)) {
		SDL_WaitThread(this->sdl_loader,NULL);
	} else {
		std::cout << "unready\n" << std::flush;
		return;
	}

	scale();
	place();
	SDL_SetSurfaceAlphaMod(this->imageSurface, this->sett.PopupOpacity * 255);
	if (this->imageTexture != NULL) {
		SDL_DestroyTexture(this->imageTexture);
		this->imageTexture = NULL;
	}
	if (imageSurface == NULL) {
		std::cout << "surface is NULL" << std::endl;
	}
	this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer,this->imageSurface);
	if (this->imageTexture == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght ,SDL_GetError());
		SDL_ClearError();
	}
	SDL_SetTextureBlendMode(this->imageTexture, SDL_BLENDMODE_BLEND);
}

void Popup::PopUp() {
	if (start.time == 0 && start.millitm == 0) {
		ftime(&start);
	}

	switch (this->Content) {
	case (GIF):
		DoGIF();
		break;
	case (IMAGE):
		DoImage();
		break;
	default:
		break;
	}
}

void Popup::DoGIF() {
	struct timeb now;
	ftime(&now);
	if (((long long)now.time * 1000 + now.millitm) - ((long long)start.time * 1000 + start.millitm) > this->lifetime) {
		if (this->sett.PopupFadeOut == true) {
			GifFadeout();
		}
	} else {
		renderGif();
	}
}

void Popup::DoImage() {
	struct timeb now;
	ftime(&now);
	if (((long long)now.time * 1000 + now.millitm) - ((long long)start.time * 1000 + start.millitm) > this->lifetime) {
		if (this->sett.PopupFadeOut == true) {
			FadeOut();
		}
	} else {
		SDL_RenderTexture(this->PopupRenderer, this->imageTexture, NULL, &this->target);
	}
}


void Popup::renderGif() {
	struct timeb now;
	ftime(&now);
	if (this->Current_image >= Gif->count) {
		this->Current_image = 0;
	}
	if (((long long)now.time * 1000 + now.millitm) - this->last_image >= Gif->delays[this->Current_image]) {
		if (this->imageTexture != NULL) {
			SDL_DestroyTexture(this->imageTexture);
			this->imageTexture = NULL;
		}
		this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer, Gif->frames[this->Current_image++]);
		SDL_SetTextureBlendMode(this->imageTexture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(this->imageTexture, this->sett.PopupOpacity * 255);
		SDL_RenderTexture(this->PopupRenderer, this->imageTexture, NULL, &this->target);
		this->last_image = (long long)now.time * 1000 + now.millitm;
	} else {
		SDL_SetTextureAlphaMod(this->imageTexture, this->sett.PopupOpacity * 255);
		SDL_RenderTexture(this->PopupRenderer, this->imageTexture, NULL, &this->target);
	}
}


void Popup::GifFadeout() {
	double dimin = this->sett.PopupOpacity / this->sett.PopupFadeOutSteps;
	double step = (this->sett.PopupFadeOutTime / this->sett.PopupFadeOutSteps);

	struct timeb middle1;
	struct timeb middle;

	ftime(&middle1);

	if (this->sett.PopupOpacity > 0.001) {
		if ((middle.time * 1000 + middle.millitm) - (middle1.time * 1000 + middle1.millitm) >= step) {
			this->sett.PopupOpacity -= dimin;
			renderGif();
		} else {
			renderGif();
		}
		middle = middle1;
	} else {
		this->death = true;
	}
}

void Popup::FadeOut() {
	double dimin = this->sett.PopupOpacity / this->sett.PopupFadeOutSteps;
	double step = (this->sett.PopupFadeOutTime / this->sett.PopupFadeOutSteps);

	struct timeb middle1;
	struct timeb middle;

	ftime(&middle1);

	if (this->sett.PopupOpacity > 0.001) {
		if ((middle.time * 1000 + middle.millitm) - (middle1.time * 1000 + middle1.millitm) >= step) {
			this->sett.PopupOpacity -= dimin;
			SDL_SetTextureAlphaMod(this->imageTexture, this->sett.PopupOpacity * 255);
			SDL_RenderTexture(this->PopupRenderer, this->imageTexture, NULL, &this->target);
		}
		middle = middle1;
	} else {
		this->death = true;
	}
}

Popup::~Popup() {
	//std::cout << "Popup vaporised" << std::endl;
	if (this->imageSurface != NULL) {
		SDL_DestroySurface(this->imageSurface);
		this->imageSurface = NULL;
	}
	if (this->imageTexture != NULL) {
		SDL_DestroyTexture(this->imageTexture);
		this->imageTexture = NULL;
	}
	if (this->Gif != NULL) {
		IMG_FreeAnimation (this->Gif);
		this->Gif = NULL;
	}
}