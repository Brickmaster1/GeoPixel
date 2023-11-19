#pragma once
#define GLEW_STATIC

#include <SDL.h>
#include <GL/glew.h>

#include "GLSLShaderProgram.h"
#include "GLTexture.h"

#include "Sprite.h"

enum class GameState {RUNNING, EXIT};

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

private:
	void init();
	void gameLoop();
	void processEvents();
	void renderGame();
	void initShaders();

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	Sprite _sprite;

	GLSLProgram _colorShaderProgram;
	GLTexture _playerTexture;

	float _time;
};