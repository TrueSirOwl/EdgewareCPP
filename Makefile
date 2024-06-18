CC = g++
CFLAGS = -g3
PopSRC = Popject/Burster.cpp Popject/ImageStorage.cpp Popject/Popup.cpp Popject/main.cpp
SettSRC = SettingsGui/AdvancedSettings.cpp SettingsGui/GeneralSettings.cpp SettingsGui/PopupSettings.cpp SettingsGui/SettGui.cpp SettingsGui/Main.cpp
SharedSRC = shared/Debug.cpp shared/Settings.cpp

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)
SDL_LIBS = -lXfixes -lX11 -lSDL2_image

FLTK_LIBS = -lfltk 

PopOBJ = $(PopSRC:.cpp=.o)
SettOBJ = $(SettSRC:.cpp=.o)
SharedOBJ = $(SharedSRC:.cpp=.o)

POPJECT = popject
SETTING = settings

all: popject settings

popject: $(PopOBJ) $(SharedOBJ)
	$(CC) $(PopOBJ) $(SharedOBJ) $(SDL_LDFLAGS) $(SDL_LIBS) -o $(POPJECT)

settings: $(SettOBJ) $(SharedOBJ)
	$(CC) $(SettOBJ) $(SharedOBJ) -o $(SETTING) $(FLTK_LIBS)

Popject/%.o: Popject/%.cpp 
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@ $(SDL_LIBS)

SettingsGui/%.o: SettingsGui/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

shared/%.o: shared/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm Popject/*.o
	-rm SettingsGui/*.o
	-rm shared/*.o
	-rm *.o

fclean: clean
	-rm $(POPJECT)
	-rm $(SETTING)
	-rm a.out

re: clean all

