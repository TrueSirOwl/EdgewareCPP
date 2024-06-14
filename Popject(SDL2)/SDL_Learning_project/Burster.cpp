#include "Burster.h"
#include <windows.h>


void SetWindowTransparent(SDL_Window* window) {
	HWND hwnd = GetConsoleWindow();
	hwnd = FindWindow(NULL, TEXT("slut"));  // Use your window's title here
	if (hwnd) {
		LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
		style |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, style);
		SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
	}
}

Burster::Burster(Settings sett, ImageStorage& src) : IMGLib(src), sett(sett) {
	getDisplays();

	this->window = SDL_CreateWindow("slut", 0, 0, this->displaySizes[0].w, this->displaySizes[0].h, SDL_WINDOW_BORDERLESS | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
	if (this->window == NULL) {
		LOG(HERROR, sett.LoggingStrenght, "Window Creation Failed. AbortingPopup");
		delete (this);
	}

	this->PopupRenderer = SDL_CreateRenderer(this->window, 2, SDL_RENDERER_ACCELERATED);
	if (this->PopupRenderer == NULL) {
		LOG(HERROR, this->sett.LoggingStrenght, "Renderer Creation Failed. AbortingPopup");
		delete(this);
	}

	SetWindowTransparent(window);

	SDL_SetRenderDrawBlendMode(PopupRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(PopupRenderer, 255, 255, 255, 255);
	SDL_RenderClear(PopupRenderer);

	SetWindowClickThrough();
	//SDL_SetWindowOpacity(this->window, 0);
	SDL_ShowWindow(this->window);
	
	Popup* testpop;
	
	std::cout << sett.BurstAmt << std::endl;
	for (int i = 0; i < sett.BurstAmt; i++)
	{
		testpop = new Popup(IMGLib, sett, displaySizes, PopupRenderer);
		std::cout << i << std::endl;
		burstBuffer.push_back(testpop->GetOutpop());
	}
	
}



	void Burster::burst() {
	std::vector<slut>::iterator it = burstBuffer.begin();
	while (it != burstBuffer.end()) {
		SDL_RenderCopy(PopupRenderer, (*it).image, NULL, &(*it).pos);
		SDL_RenderPresent(this->PopupRenderer);
		++it;
	}
	SDL_ShowWindow(this->window);
}

void Burster::getDisplays() {
	this->displayNumber = SDL_GetNumVideoDisplays();
	for (int i = 0; i < this->displayNumber; i++) {
		this->displaySizes.push_back(SDL_Rect());
		SDL_GetDisplayBounds(i, &this->displaySizes.back());
	}
}

#if defined(_WIN32)
#include <windows.h>
#include <SDL_syswm.h>
#endif


void Burster::SetWindowClickThrough() {
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if (SDL_GetWindowWMInfo(window, &wmInfo)) {

#if defined(_WIN32)

		LOG(INFO, this->sett.LoggingStrenght, "Activting Clickthrough for Windows");
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

Burster::~Burster() {
}