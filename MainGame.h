#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

#include <ViXeL/rendering/GLSLShaderProgram.h>
#include <ViXeL/rendering/Sprite.h>
#include <ViXeL/Window.h>
#include <ViXeL/rendering/camera/Camera.h>
#include <ViXeL/rendering/SpriteBatch.h>
#include <ViXeL/logic/InputManager.h>
#include <ViXeL/logic/Timing.h>

#include "Projectile.h"
#include "data/ResourceManager.h"

enum class GameState {RUNNING, EXIT};

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

    static GameState gameState;

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

	ViXeL::GLSLProgram _colorShaderProgram;

	ViXeL::Camera<ViXeL::OrthoCamera2D> _camera;

	ViXeL::SpriteBatch _spriteBatch;

	ViXeL::FpsLimiter _fpsLimiter;

	std::vector<std::string> _loadedTextures;

	std::vector<Projectile> _projectiles;

	glm::vec2 _playerPos;

	float _fps;
	float _maxFps;
	int _vsync;
	float _gameTime;

    bool _resourceMode;
};