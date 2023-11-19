#include "MainGame.h"
#include "Errors.h"
#include "ImageLoader.h"

#include <iostream>
#include <chrono>

MainGame::MainGame() : 
	_screenWidth(1024), 
	_screenHeight(768), 
	_time(0.0f), 
	_window(nullptr), 
	_gameState(GameState::RUNNING) 
{}

MainGame::~MainGame() {}

void MainGame::run() {
	init();

	_sprite.create(-1.0f, -1.0f, 2.0f, 2.0f);

	_playerTexture = ImageLoader::loadPNG("Textures/Test_PixelCube.png");

	gameLoop();
}

void MainGame::init() {
	//Init all SDL subsystems
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("GeoPixel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (_window == nullptr) {
		fatalError("SDL_GL_Context could not be created!");
	}
	
	int error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize GLEW!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	std::cout << glGetString(GL_VENDOR) << '\n';
	std::cout << glGetString(GL_RENDERER) << '\n';
	std::cout << glGetString(GL_VERSION) << '\n';

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	initShaders();
}

void MainGame::initShaders() {
	_colorShaderProgram.compileShaders("Shaders/colorShader.vsh", "Shaders/colorShader.fsh");
	_colorShaderProgram.addAttribute("vertexPosition");
	_colorShaderProgram.addAttribute("vertexColor");
	_colorShaderProgram.addAttribute("vertexUV");
	_colorShaderProgram.linkShaders();

	//if (_colorShaderProgram.validateShaderProgram(true) == false) {
	//	fatalError("Shader program validation failed!");
	//}
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processEvents();
		_time += 0.01;
		renderGame();
	}
}

void MainGame::processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				
				break;
		}
	}
}

void MainGame::renderGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorShaderProgram.bindShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	GLint textureLocation = _colorShaderProgram.getUniformLocation("textureSampler");
	glUniform1i(textureLocation, 0);

	GLint timeLocation = _colorShaderProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.render();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorShaderProgram.unbindShaderProgram();

	SDL_GL_SwapWindow(_window);
}