#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

#include <ViXeL/rendering/GLSLShaderProgram.h>
#include <ViXeL/rendering/Sprite.h>
#include <ViXeL/Window.h>
#include <ViXeL/camera/Camera2D.h>
#include <ViXeL/rendering/SpriteBatch.h>
#include <ViXeL/logic/InputManager.h>
#include <ViXeL/logic/Timing.h>

#include "Projectile.h"

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
	void processPhysics();
	void renderGame();
	void initShaders();
	//void getFps();

	ViXeL::Window _window;
	int _windowWidth;
	int _windowHeight;
	GameState _gameState;

	ViXeL::GLSLProgram _colorShaderProgram;

	ViXeL::Camera2D _camera;

	ViXeL::SpriteBatch _spriteBatch;
	
	ViXeL::FpsLimiter _fpsLimiter;

	ViXeL::InputManager _inputManager;

	std::vector<std::string> _loadedTextures;

	std::vector <Projectile> _projectiles;

	glm::vec2 _playerPos;

	float _fps;
	float _maxFps;
	int _vsync;
	float _gameTime;
};