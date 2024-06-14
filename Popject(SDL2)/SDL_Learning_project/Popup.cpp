#include "Popup.hpp"
#include <random>
#include <algorithm>
#include <chrono>
#include <Debug.h>

Popup::Popup(ImageStorage& src, const Settings popsett, SDL_Renderer* PopupRenderer, SDL_Window* window, std::vector<SDL_Rect> displaySizes): sett(popsett), ImageLib(src), lifetime(sett.PopupLifespan), death(false), born(false), window(window), PopupRenderer(PopupRenderer),displaySizes(displaySizes){
	/*
	getDisplays();
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
	if (this->window == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Window Creation Failed. AbortingPopup");
		delete (this);
	}

	if (this->sett.Overlay == 1) {
		SetWindowClickThrough();
	}
	
	this->PopupRenderer = SDL_CreateRenderer(this->window, 2 ,SDL_RENDERER_ACCELERATED);
	if (this->PopupRenderer == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Renderer Creation Failed. AbortingPopup");
		delete(this);
	}
	*/
	getImage();
	scaleImage();
	placer();
	SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}


Popup::Popup(ImageStorage& src, const Settings popsett, std::vector<SDL_Rect> displaySizes, SDL_Renderer* PopupRenderer) : sett(popsett), ImageLib(src), lifetime(sett.PopupLifespan), death(false), born(false), window(NULL), PopupRenderer(PopupRenderer), displaySizes(displaySizes) {
	/*
	getDisplays();
	this->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
	if (this->window == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Window Creation Failed. AbortingPopup");
		delete (this);
	}

	if (this->sett.Overlay == 1) {
		SetWindowClickThrough();
	}

	this->PopupRenderer = SDL_CreateRenderer(this->window, 2 ,SDL_RENDERER_ACCELERATED);
	if (this->PopupRenderer == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Renderer Creation Failed. AbortingPopup");
		delete(this);
	}
	*/
	getImage();

	SDL_SetTextureBlendMode(this->outpop.image, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(this->outpop.image, 128);

	scaleImage();
	placer();
	//SDL_SetWindowOpacity(this->window, this->sett.PopupOpacity);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}

/*
void Popup::getDisplays() {
	this->displayNumber = SDL_GetNumVideoDisplays();
	for ( int i = 0; i < this->displayNumber; i++) {
		this->displaySizes.push_back(SDL_Rect());
		SDL_GetDisplayBounds(i, &this->displaySizes.back());
	}

}
*/

	/*
	while (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) != "gif") {
		this->ContentPath = ImageLib.getRandomImage();
		std::cout << this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) << std::endl;
	}
	*/

void Popup::getImage() {
	this->ContentPath = ImageLib.getRandomImage();
	std::cout << this->ContentPath << std::endl;
	this->outpop.type = getContentType();
	if (this->outpop.type == GIF) {
		this->outpop.gif = IMG_LoadAnimation(this->ContentPath.c_str());
	}
	else {
		this->outpop.image = IMG_LoadTexture(this->PopupRenderer, this->ContentPath.c_str());
	}
	//==================safety LOOP======================//
	while (this->outpop.image == NULL && this->outpop.gif == NULL) {
		if (this->outpop.type == GIF) {
			this->outpop.gif = IMG_LoadAnimation(this->ContentPath.c_str());
		} else {
			this->outpop.image = IMG_LoadTexture(this->PopupRenderer, this->ContentPath.c_str());
		}
		LOG(WARNING, this->sett.LoggingStrenght, "Loading " + this->ContentPath + " Failed: " + (std::string)SDL_GetError() + ", trying diferent image");
		SDL_ClearError();
		this->ContentPath = ImageLib.getRandomImage();
		std::cout << this->ContentPath << std::endl;
	}
	//===================================================//
	if (SDL_QueryTexture(this->outpop.image, NULL, NULL, &this->outpop.pos.w, &this->outpop.pos.h) != 0) {
		LOG(HERROR, this->sett.LoggingStrenght, "could not get dimensions of " + this->ContentPath);
	}
}

ContentType Popup::getContentType() {
	if (this->ContentPath.substr(this->ContentPath.find_last_of('.') + 1) == "gif") {
		LOG(INFO, this->sett.LoggingStrenght, this->ContentPath + " treated as GIF");
		return(GIF);
	}
	else {
		LOG(INFO, this->sett.LoggingStrenght, this->ContentPath + " treated as IMAGE");
		return (IMAGE);
	}
}

void Popup::scaleImage() {
	
	double sourceSize, targetSize;

	std::random_device rd;
	std::default_random_engine randomizerEngine(rd());
	std::uniform_real_distribution<double> scaleFactor(0.7, 1);
	sourceSize = 1 + (std::max<int>(this->outpop.pos.w, this->outpop.pos.h) / std::min<int>(this->displaySizes[0].w, this->displaySizes[0].h));
	sourceSize = 1 + (std::max<int>(this->outpop.pos.w, this->outpop.pos.h) / std::min<int>(this->displaySizes[0].w, this->displaySizes[0].h));

	targetSize = scaleFactor(randomizerEngine);
	this->resizeFactor = targetSize / sourceSize;
	this->outpop.pos.w = static_cast<int>(this->outpop.pos.w * resizeFactor);
	this->outpop.pos.h = static_cast<int>(this->outpop.pos.h * resizeFactor);

	//SDL_SetWindowSize(this->window, this->target.w, this->target.h);
	
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
//std::uniform_int_distribution<int> WhereH(0, this->displaySizes[0].h - this->target.h);
	//std::uniform_int_distribution<int> WhereW(0, this->displaySizes[0].w - this->target.w);
	std::uniform_int_distribution<int> WhereH(0, this->displaySizes[0].h - this->target.h);
	std::uniform_int_distribution<int> WhereW(0, this->displaySizes[0].w - this->target.w);
	this->outpop.pos.x = WhereW(randomizerEngine);
	this->outpop.pos.y = WhereH(randomizerEngine);
	SDL_SetWindowPosition(this->window, this->outpop.pos.x, this->outpop.pos.y);
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
	int Gif_length = this->outpop.gif->count;
	if (this->Current_image >= Gif_length) {
		this->Current_image = 0;
	}
	if (((long long)now.time * 1000 + now.millitm) - last_image >= this->outpop.gif->delays[this->Current_image]) {
		SDL_DestroyTexture(this->outpop.image);
		this->outpop.image = SDL_CreateTextureFromSurface(this->PopupRenderer, this->outpop.gif->frames[this->Current_image++]);
		SDL_RenderCopy(this->PopupRenderer, this->outpop.image, NULL, NULL);
		SDL_RenderPresent(this->PopupRenderer);
		last_image = (long long)now.time * 1000 + now.millitm;
	}
}

void Popup::renderImage() {
	//SDL_DestroyTexture(this->outpop.image);
	//this->outpop.image = SDL_CreateTextureFromSurface(this->PopupRenderer,this->imageSurface);
	//SDL_FreeSurface(this->imageSurface);
	SDL_RenderCopy(this->PopupRenderer, this->outpop.image, NULL, NULL);
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

const slut Popup::GetOutpop() {
	return (this->outpop);
}

#if defined(_WIN32)
#include <windows.h>
#include <SDL_syswm.h>
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

			// Set the window to be transparent
			Atom opacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);
			unsigned long value = (unsigned long)(0.0 * 0xFFFFFFFF); // Fully transparent
			XChangeProperty(display, xwindow, opacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&value, 1);

			// Set the window to allow mouse clicks to pass through
			XSetWindowAttributes attrs;
			attrs.override_redirect = True;
			XChangeWindowAttributes(display, xwindow, CWOverrideRedirect, &attrs);

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