#include "MainGame.h"

int main(int argc, char** argv) {
	MainGame mainGame;
	mainGame.setMaxFps(60);
    mainGame.run();

	return 0;
}