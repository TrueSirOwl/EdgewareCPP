#include "SettGui.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
	SettGui* G;
	if (argc < 2) {
		G = new SettGui;
	} else {
		G = new SettGui(argv[1]);
	}
	return(Fl::run());
}