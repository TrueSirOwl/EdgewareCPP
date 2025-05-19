CC = g++
WCC = x86_64-w64-mingw32-g++-posix
CFLAGS = -g3 -std=c++17

WINFLAGS = -mconsole -lsetupapi -lole32 -loleaut32 -lgdi32 -limm32 -lversion -lwinmm -static

PopSRC = Popject/Burster.cpp Popject/ImageStorage.cpp Popject/Popup.cpp Popject/main.cpp
SettSRC = SettingsGui/AdvancedSettings.cpp SettingsGui/GeneralSettings.cpp SettingsGui/PopupSettings.cpp SettingsGui/SettGui.cpp SettingsGui/Main.cpp
SharedSRC = shared/Debug.cpp shared/Settings.cpp

Winclude = -I/usr/x86_64-w64-mingw32/include -I/usr/x86_64-w64-mingw32/include/SDL2
SDL_WINLIBS = -lSDL2_image -lSDL2

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)
SDL_LIBS = -lXfixes -lX11 -lSDL2_image

FLTK_LIBS = -lfltk

PopOBJ = $(PopSRC:.cpp=.g++.o)
SettOBJ = $(SettSRC:.cpp=.g++.o)
SharedOBJ = $(SharedSRC:.cpp=.g++.o)

winPopOBJ = $(PopSRC:.cpp=.mingw.o)
winSettOBJ = $(SettSRC:.cpp=.mingw.o)
winSharedOBJ = $(SharedSRC:.cpp=.mingw.o)

POPJECT = popject
SETTING = settings

WINPOPJECT = popject.exe
WINSETTING = settings.exe

all: popject settings

windows: winpopject winsettings


popject: $(PopOBJ) $(SharedOBJ)
	$(CC) $(PopOBJ) $(SharedOBJ) $(SDL_LDFLAGS) $(SDL_LIBS) -o $(POPJECT)

settings: $(SettOBJ) $(SharedOBJ)
	$(CC) $(SettOBJ) $(SharedOBJ) -o $(SETTING) $(FLTK_LIBS)

Popject/%.g++.o: Popject/%.cpp 
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@ $(SDL_LIBS)

SettingsGui/%.g++.o: SettingsGui/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

shared/%.g++.o: shared/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@


winpopject: $(winPopOBJ) $(winSharedOBJ)
	$(WCC) $(winPopOBJ) $(winSharedOBJ) $(SDL_WINLIBS) -o $(WINPOPJECT) $(WINFLAGS) 

winsettings: $(winSettOBJ) $(winSharedOBJ)
	$(WCC) $(winSettOBJ) $(winSharedOBJ) -o $(WINSETTING) $(FLTK_LIBS) $(WINFLAGS)

Popject/%.mingw.o: Popject/%.cpp 
	$(WCC) $(CFLAGS) $(Winclude) -c $< -o $@ $(SDLWIN_LIBS)

SettingsGui/%.mingw.o: SettingsGui/%.cpp
	$(WCC) $(CFLAGS) $(Winclude) -c $< -o $@

shared/%.mingw.o: shared/%.cpp
	$(WCC) $(CFLAGS) $(Winclude) -c $< -o $@


clean:
	-rm Popject/*.o
	-rm SettingsGui/*.o
	-rm shared/*.o
	-rm *.o

fclean: clean
	-rm $(POPJECT)
	-rm $(SETTING)
	-rm $(WINPOPJECT)
	-rm $(WINSETTING)
	-rm a.out

re: fclean all

wre : fclean windows

