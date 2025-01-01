#include "Popup.hpp"
#include <random>
#include <algorithm>
#include <chrono>
#include <SDL_syswm.h>

Popup::Popup(ImageStorage& src, const Settings popsett): 
sett(popsett), ImageLib(src), lifetime(sett.PopupLifespan), death(false), born(false),
Current_image(0), imageSurface(NULL), imageTexture(NULL), Gif(NULL), Content(IMAGE), last_image(0) {
	getDisplays();
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
	if (this->window == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Window Creation Failed. AbortingPopup");
		delete (this);
	}
	
	this->PopupRenderer = SDL_CreateRenderer(this->window, -1 ,SDL_RENDERER_ACCELERATED);
	if (this->PopupRenderer == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Renderer Creation Failed. AbortingPopup");
		this->CheckDeath.lock();
		this->death = true;
		this->CheckDeath.unlock();
	}

	this->loader = std::thread(&Popup::getImage,this);
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

	if (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) == "gif") {
		this->Gif = IMG_LoadAnimation(this->ContentPath.c_str());
		this->Content = GIF;
		std::cout << "loading gif" << std::endl;
	}
		this->imageSurface = IMG_Load(this->ContentPath.c_str());


	if (this->imageSurface == NULL) {
		LOG(WARNING, this->sett.LoggingStrenght, "Loading " + this->ContentPath + " Failed: " + (std::string)SDL_GetError());
		exit(1);
	}
}

void Popup::scaleWindow() {
	
	double sourceSize, targetSize;

	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_real_distribution<double> scaleFactor(this->sett.ImageSizeMin, this->sett.ImageSizeMax);
	sourceSize = 1 + (std::max<int>(this->imageSurface->w, this->imageSurface->h) / std::min<int>(this->displaySizes[0].w, this->displaySizes[0].h));
	//sourceSize = 1 + (std::max<int>(this->imageSurface->w, this->imageSurface->h) / std::min<int>(1920, 1080));

	targetSize = scaleFactor(randomizerEngine);
	this->resizeFactor = targetSize / sourceSize;
	this->target.w = static_cast<int>(this->imageSurface->w * resizeFactor);
	this->target.h = static_cast<int>(this->imageSurface->h * resizeFactor);

	SDL_SetWindowSize(this->window, this->target.w, this->target.h);
}

void Popup::placeWindow() {
	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_int_distribution<int> WhereH(0, this->displaySizes[0].h - this->target.h);
	std::uniform_int_distribution<int> WhereW(0, this->displaySizes[0].w - this->target.w);
	//std::uniform_int_distribution<int> WhereH(0, 1080 - this->target.h);
	//std::uniform_int_distribution<int> WhereW(0, 1920 - this->target.w);
	this->ImageLocX = WhereW(randomizerEngine);
	this->ImageLocY = WhereH(randomizerEngine);
	SDL_SetWindowPosition(this->window, this->ImageLocX, this->ImageLocY);
}

void Popup::PopUp() {
	try
	{
		if (this->loader.joinable() == true) {
			this->loader.join();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	scaleWindow();
	placeWindow();
	SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	
	if (this->sett.Overlay == 1) {
		SetWindowClickThrough();
	}

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
		if (this->sett.PopupLifespan >= 0) {
			run = std::thread(&Popup::ImageThread, this);
			run.detach();
		}
		break;
	default:
		break;
	}
	SDL_ShowWindow(this->window);
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
	this->CheckDeath.lock();
	this->death = true;
	this->CheckDeath.unlock();

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
	this->CheckDeath.lock();
	this->death = true;
	this->CheckDeath.unlock();
}

void Popup::ImageThread() {
	renderImage();
	SDL_FreeSurface(this->imageSurface);
	std::this_thread::sleep_for(std::chrono::milliseconds(this->lifetime));
	//std::cout << "age kill" << std::endl;
	if (this->sett.PopupFadeOut == true) {
		FadeOut();
	}
	this->CheckDeath.lock();
	this->death = true;
	this->CheckDeath.unlock();
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
		}
		this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer, Gif->frames[this->Current_image++]);
		SDL_RenderCopy(this->PopupRenderer, this->imageTexture, NULL, NULL);
		SDL_RenderPresent(this->PopupRenderer);
		this->last_image = (long long)now.time * 1000 + now.millitm;
	}
}

void Popup::renderImage() {
	if (this->imageTexture != NULL) {
		SDL_DestroyTexture(this->imageTexture);
	}
	if (imageSurface == NULL) {
		std::cout << "surface is NULL" << std::endl;
	}
	this->imageTexture = SDL_CreateTextureFromSurface(this->PopupRenderer,this->imageSurface);
	if (this->imageTexture == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght ,SDL_GetError());
		SDL_ClearError();
	}
	int renderError = SDL_RenderClear(this->PopupRenderer);
	if (renderError != 0) {
		LOG(HERROR, this->sett.LoggingStrenght ,SDL_GetError());
		SDL_ClearError();
	}
	SDL_RenderCopy(this->PopupRenderer, this->imageTexture, NULL, NULL);
	SDL_RenderPresent(this->PopupRenderer);
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

#if defined(_WIN32)
#include <windows.h>
#include <SDL_syswm.h>
#endif

#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#endif

	void Popup::SetWindowClickThrough() {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		if (SDL_GetWindowWMInfo(window, &wmInfo)) {

#if defined(_WIN32)

			LOG(INFO,this->sett.LoggingStrenght,"Activting Clickthrough for Windows");
			HWND hwnd = wmInfo.info.win.window;
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
#endif

#if defined(__APPLE__)

			LOG(INFO, this->sett.LoggingStrenght, "Activting Clickthrough for Apple");
			NSWindow* nswindow = wmInfo.info.cocoa.window;
			[nswindow setIgnoresMouseEvents : YES] ;
			[nswindow setCanBecomeKeyWindow : NO] ;
			[nswindow setCanBecomeMainWindow : NO] ;
#endif

#if defined(__linux__)
			LOG(INFO, this->sett.LoggingStrenght, "Activting Clickthrough for Linux");
			Display* display = wmInfo.info.x11.display;
			Window xwindow = wmInfo.info.x11.window;

			XRectangle rect;
			XserverRegion region = XFixesCreateRegion(display, &rect, 1);
			XFixesSetWindowShapeRegion(display, xwindow, ShapeInput, 0, 0, region);
			XFixesDestroyRegion(display, region);
#endif
		}
		else {
			LOG(HERROR, this->sett.LoggingStrenght, "SDL_GetWindowWMInfo Error : " + static_cast<std::string>(SDL_GetError()));
		}
	}