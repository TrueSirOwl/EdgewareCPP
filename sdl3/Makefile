CXX = g++
CXXFLAGS = -g3 -std=c++23 -Ishared

PopSRC = Popject/main.cpp Popject/ImageStorage.cpp Popject/Popup.cpp Popject/Burster.cpp
SettSRC = SettingsGui/AdvancedSettings.cpp SettingsGui/GeneralSettings.cpp SettingsGui/PopupSettings.cpp SettingsGui/SettGui.cpp SettingsGui/Main.cpp
SharedSRC = shared/Debug.cpp shared/Settings.cpp


SDL_CXXFLAGS = #$(shell sdl3-config --cflags)
SDL_LDFLAGS = #$(shell sdl3-config --libs)
SDL_LIBS = -lXfixes -lX11 -lSDL3_image -lSDL3

FLTK_LIBS = -lfltk

PopOBJ = $(PopSRC:.cpp=.g++.o)
SettOBJ = $(SettSRC:.cpp=.g++.o)
SharedOBJ = $(SharedSRC:.cpp=.g++.o)

POPJECT = popject
SETTING = settings

all: popject settings

popject: $(PopOBJ) $(SharedOBJ)
	$(CXX) $(PopOBJ) $(SharedOBJ) $(SDL_LDFLAGS) $(SDL_LIBS) -o $(POPJECT)

settings: $(SettOBJ) $(SharedOBJ)
	$(CXX) $(SettOBJ) $(SharedOBJ) -o $(SETTING) $(FLTK_LIBS)

Popject/%.g++.o: Popject/%.cpp 
	$(CXX) $(CXXFLAGS) $(SDL_CXXFLAGS) -c $< -o $@ $(SDL_LIBS)

SettingsGui/%.g++.o: SettingsGui/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

shared/%.g++.o: shared/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm Popject/*.o
	-rm SettingsGui/*.o
	-rm shared/*.o
	-rm *.o

fclean: clean
	-rm $(POPJECT)
	-rm $(SETTING)
	-rm a.out
	-rm logs/*

re: fclean all

