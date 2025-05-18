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
#include "func.hpp"

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

	SetWindowClickThrough(window);

	std::cout << "properties: "<< SDL_GetWindowProperties (window) << std::endl;

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


#if defined(_WIN32)
#include <windows.h>
#include <SDL_syswm.h>
#endif

#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/shape.h>
#endif

void SetWindowClickThrough(SDL_Window* window) {

#if defined(SDL_PLATFORM_WIN32)
	HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
	if (hwnd) {
			SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE);
	}

#elif defined(SDL_PLATFORM_MACOS)
	NSWindow *nswindow
	NSWindow* nswindow = wmInfo.info.cocoa.window;
	[nswindow setIgnoresMouseEvents : YES] ;
	[nswindow setCanBecomeKeyWindow : NO] ;
	[nswindow setCanBecomeMainWindow : NO] ;

#elif defined(SDL_PLATFORM_LINUX)
	std::cout << "trying to activate clickthough" << std::endl;
	if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
		Display *xdisplay = (Display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
		Window xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
		if (xdisplay && xwindow) {
			XWMHints *hints = XAllocWMHints();
			hints->flags = InputHint;
			hints->input = False;  // Prevents window from taking focus

			XSetWMHints(xdisplay, xwindow, hints);
			XFree(hints);

			XRectangle rect;
			XserverRegion region = XFixesCreateRegion(xdisplay, &rect, 1);
			XFixesSetWindowShapeRegion(xdisplay, xwindow, ShapeInput, 0, 0, region);
			XFixesDestroyRegion(xdisplay, region);
		}
	}
#elif
	LOG(HERROR, this->sett.LoggingStrenght, "SDL_GetWindowWMInfo Error : " + static_cast<std::string>(SDL_GetError()));
#endif
	}