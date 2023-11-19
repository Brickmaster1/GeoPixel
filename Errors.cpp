#include "Errors.h"

#include <iostream>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <SDL.h>

void fatalError(std::string errorString)  {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit.";
	_getch();
	SDL_Quit();
	std::exit(EXIT_FAILURE);
}