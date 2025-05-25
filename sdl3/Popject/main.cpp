#define SDL_MAIN_HANDLED

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
#include <queue>
#include "Burster.hpp"

int main(int argc, char* argv[]) {
	
	std::string loc;
	if (argc < 2) {
		loc = "./shared/Settings.txt";
	} else {
		loc = argv[1];
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	Settings* Sett = ReadSettings(loc.c_str());
	CreateLogFile();
	
	signal(SIGINT, SIG_DFL);
	if (Sett->ImageFolderPath.empty() == true ) {
		std::cerr << "No Image path specified!" << std::endl;
		return(1);
	}
	ImageStorage IMGLib = ImageStorage(Sett->ImageFolderPath);
	LOG(INFO, Sett->LoggingStrenght , "Getting Images from : "+ Sett->ImageFolderPath);
	
	SDL_Window* window;
	SDL_Renderer* renderer;

	//create an array containing available screens
	int dispnum;
	SDL_DisplayID* disps;

	disps = SDL_GetDisplays(&dispnum);
	SDL_Rect dispbounds[dispnum];
	int c = 0;
	while(c < dispnum) {
		SDL_GetDisplayBounds(disps[c],&dispbounds[c]);
		++c;
	}

	//Initialize window and renderer
	SDL_CreateWindowAndRenderer("title",dispbounds[0].w, dispbounds[0].h, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_NOT_FOCUSABLE| SDL_WINDOW_BORDERLESS, &window,&renderer);

	//set up and display window and renderer
	SetWindowClickThrough(window);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	
	std::queue<Burster*> preps;

	std::vector<Burster*> active;
	std::vector<Burster*>::iterator it;

	std::queue<Burster*>trash;

	//intializes the queue with two bursters to have a backlocg that can load their images in the background
	preps.push(new Burster(IMGLib, *Sett, dispbounds, window, renderer));
	preps.push(new Burster(IMGLib, *Sett, dispbounds, window, renderer));

	timeb start;
	timeb end {};
	
	ftime(&start);
	while (true) {
		SDL_RenderClear(renderer);
		if (((long long)end.time * 1000 + end.millitm) - ((long long)start.time * 1000 + start.millitm) > Sett->TimeBetweenPopups) {
			if (preps.front()->prep() == true) {
				active.push_back(preps.front());
				preps.pop();
				preps.push(new Burster(IMGLib, *Sett, dispbounds, window, renderer));
				ftime(&start);
			}
		}

		it = active.begin();
		while (it != active.end()) {
			(*it)->burst();
			if ((*it)->checkBurstDone() == true) {
				delete (*it);
				active.erase(it);
			} else {
				++it;
			}
		}
		SDL_RenderPresent(renderer);
		ftime(&end);
	}

	// cleanup
	
	while (preps.empty() == false ) {
		preps.pop();
	}
	delete(Sett);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return (0);
}


#if defined(_WIN32)
#include <windows.h>
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