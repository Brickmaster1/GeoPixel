#pragma once

#include <SDL.h>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <ViXeL/Window.h>
#include <ViXeL/data/ResourceManager.h>
#include <ViXeL/logic/InputManager.h>
#include <ViXeL/logic/Timing.h>
#include <ViXeL/model/Mesh.h>
#include <ViXeL/rendering/GLSLShaderProgram.h>
#include <ViXeL/rendering/Sprite.h>
#include <ViXeL/rendering/SpriteBatcher.h>
#include <ViXeL/rendering/camera/Camera.h>
#include <ViXeL/util/Templating.h>
#include <ViXeL/util/Typing.h>

#include "Projectile.h"

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

    ViXeL::GLSLProgram _testShaderProgram;

    ViXeL::Camera<ViXeL::PerspectiveCamera3D> _testCamera3d;

    ViXeL::Camera<ViXeL::Camera2D> _testCamera2d;

    using CameraSelection = type_selection<ViXeL::Camera<ViXeL::Camera2D>, ViXeL::Camera<ViXeL::OrthoCamera3D>, ViXeL::Camera<ViXeL::PerspectiveCamera3D>>;
    std::vector<CameraSelection> _cameras;

	ViXeL::SpriteBatcher _spriteBatch;

	ViXeL::FpsLimiter _fpsLimiter;

	std::vector<std::filesystem::path> _loadedTextures;

	std::vector<Projectile> _projectiles;

    ViXeL::Mesh<ViXeL::ColoredTexturedVertex3D> cubeMesh;
    std::vector<glm::vec3> cubePositions;

    bool mouseMoved = false;
    bool scrolled = false;
    bool keyPressed = false;
    bool mouseButtonPressed = false;

    bool render2dEnabled = false;

	glm::vec2 _playerPos;

	float _fps;
	float _maxFps;
	int _vsync;
	float _gameTime;

    bool _resourceMode;
};
