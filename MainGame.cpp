#include "MainGame.h"

#include "AutoLoader.h"
#include <ViXeL/ViXeL.h>
#include <ViXeL/error/Errors.h>
#include <ViXeL/data/ResourceManager.h>
#include <ViXeL/data/Vertex.h>

MainGame::MainGame() : 
	_windowWidth(1024),
	_windowHeight(768),
	_gameState(GameState::RUNNING),
	_playerPos(glm::vec2(0.0f, 0.0f)),
	_maxFps(165.0f),
	_vsync(1),
	_gameTime(0.0f)
{
	_camera.init(_windowWidth, _windowHeight);
}

MainGame::~MainGame() {}

void MainGame::run() {
	init();
	
	gameLoop();
}

void MainGame::init() {
	ViXeL::init("GeoPixel");

	_camera.init(_windowWidth, _windowHeight);

	_window.create("GeoPixel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);
	
	std::cout << glGetString(GL_VENDOR) << '\n';
	std::cout << glGetString(GL_RENDERER) << '\n';
	std::cout << glGetString(GL_VERSION) << '\n';

	//Set VSYNC
	SDL_GL_SetSwapInterval(_vsync);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	_fpsLimiter.init(_maxFps);

	initShaders();

	_spriteBatch.init();

	fetchSprites(&_loadedTextures);
}

void MainGame::initShaders() {
	_colorShaderProgram.compileShaders("Shaders/colorShader.vsh", "Shaders/colorShader.fsh");
	_colorShaderProgram.addAttribute("vertexPosition");
	_colorShaderProgram.addAttribute("vertexColor");
	_colorShaderProgram.addAttribute("vertexUV");
	_colorShaderProgram.linkShaders();
}

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		//Used for frame time measure
		float startTicks = SDL_GetTicks64();
		
		processEvents();

		_gameTime += 0.01;

		_camera.update();

		processPhysics();

		renderGame();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10) {
			std::cout << "Current fps: " << _fps << "\r";
			frameCounter = 0;
		}

		_fps = _fpsLimiter.endFrame();
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
				_inputManager.onMouseMove(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.onKeyDown(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.onKeyUp(event.button.button);
				break;
			case SDL_KEYDOWN:
				_inputManager.onKeyDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.onKeyUp(event.key.keysym.sym);
				break;
		}
	}

	const float CAMERA_PAN_SPEED = 10.0f;
	const float CAMERA_ZOOM_SPEED = 0.05f;

	if (_inputManager.getKeyState(SDLK_e)) { _camera.setScale(_camera.getScale() + CAMERA_ZOOM_SPEED); }

	if (_inputManager.getKeyState(SDLK_q)) { _camera.setScale(_camera.getScale() - CAMERA_ZOOM_SPEED); }

	if (_inputManager.getKeyState(SDLK_w)) { _camera.setPos(_camera.getPos() + glm::vec2(0.0f, CAMERA_PAN_SPEED)); }

	if (_inputManager.getKeyState(SDLK_a)) { _camera.setPos(_camera.getPos() + glm::vec2(-CAMERA_PAN_SPEED, 0.0f)); }

	if (_inputManager.getKeyState(SDLK_s)) { _camera.setPos(_camera.getPos() + glm::vec2(0.0f, -CAMERA_PAN_SPEED)); }

	if (_inputManager.getKeyState(SDLK_d)) { _camera.setPos(_camera.getPos() + glm::vec2(CAMERA_PAN_SPEED, 0.0f)); }

	if (_inputManager.getKeyState(SDL_BUTTON_LEFT)) {
		glm::vec2 mousePos = _inputManager.getMousePos();
		mousePos = _camera.toWorldPos(mousePos);

		glm::vec2 direction = mousePos - _playerPos;
		direction = glm::normalize(direction);

		_projectiles.emplace_back(_playerPos, direction, 0.125f, 1000, glm::vec4(-15.0f, -15.0f, 15.0f, 15.0f));
	}

	if (_inputManager.getKeyState(SDL_BUTTON_RIGHT)) {
		_playerPos = _camera.toWorldPos(_inputManager.getMousePos());
	}

	if (_inputManager.getKeyState(SDLK_y)) {
		for (int i = 0; i < _projectiles.size(); i++) {
			_projectiles.pop_back();
		}
	}
}

void MainGame::processPhysics() {
	//Update physics state every frame
	for (int i = 0; i < _projectiles.size();) {
		//Check for collisions
		/*for (int j = 0; j < _projectiles.size(); j++) {
			if (i == j) continue;
			//glm::vec2 compareVec = _projectiles[i].getPos() - _projectiles[j].getPos();
			
			glm::vec2 posI2 = _projectiles[i].getPos() + _projectiles[i].getMotionVec();
			glm::vec2 posJ2 = _projectiles[i].getPos() + _projectiles[i].getMotionVec();

			glm::vec4 lineI1 = glm::vec4((_projectiles[i].getHitbox())[0] + _projectiles[i].getPos().x, (_projectiles[i].getHitbox())[1] + _projectiles[i].getPos().y, (_projectiles[i].getHitbox())[0] + posI2.x, (_projectiles[i].getHitbox())[1] + posI2.y);
			glm::vec4 lineI2 = glm::vec4((_projectiles[i].getHitbox())[2] + _projectiles[i].getPos().x, (_projectiles[i].getHitbox())[3] + _projectiles[i].getPos().y, (_projectiles[i].getHitbox())[2] + posI2.x, (_projectiles[i].getHitbox())[3] + posI2.y);
			glm::vec4 lineI3 = glm::vec4((_projectiles[i].getHitbox())[0] + _projectiles[i].getPos().x, (_projectiles[i].getHitbox())[3] + _projectiles[i].getPos().y, (_projectiles[i].getHitbox())[0] + posI2.x, (_projectiles[i].getHitbox())[3] + posI2.y);
			glm::vec4 lineI4 = glm::vec4((_projectiles[i].getHitbox())[2] + _projectiles[i].getPos().x, (_projectiles[i].getHitbox())[0] + _projectiles[i].getPos().y, (_projectiles[i].getHitbox())[2] + posI2.x, (_projectiles[i].getHitbox())[0] + posI2.y);

			glm::vec4 lineJ1 = glm::vec4((_projectiles[j].getHitbox())[0] + _projectiles[j].getPos().x, (_projectiles[j].getHitbox())[1] + _projectiles[j].getPos().y, (_projectiles[j].getHitbox())[0] + posJ2.x, (_projectiles[j].getHitbox())[1] + posJ2.y);
			glm::vec4 lineJ2 = glm::vec4((_projectiles[j].getHitbox())[2] + _projectiles[j].getPos().x, (_projectiles[j].getHitbox())[3] + _projectiles[j].getPos().y, (_projectiles[j].getHitbox())[2] + posJ2.x, (_projectiles[j].getHitbox())[3] + posJ2.y);
			glm::vec4 lineJ3 = glm::vec4((_projectiles[j].getHitbox())[0] + _projectiles[j].getPos().x, (_projectiles[j].getHitbox())[3] + _projectiles[j].getPos().y, (_projectiles[j].getHitbox())[0] + posJ2.x, (_projectiles[j].getHitbox())[3] + posJ2.y);
			glm::vec4 lineJ4 = glm::vec4((_projectiles[j].getHitbox())[2] + _projectiles[j].getPos().x, (_projectiles[j].getHitbox())[0] + _projectiles[j].getPos().y, (_projectiles[j].getHitbox())[2] + posJ2.x, (_projectiles[j].getHitbox())[0] + posJ2.y);

			//for () {

			//}
			
			
			if (compareVec.x <= 1.0f || compareVec.y <= 1.0f) {
				_projectiles[i].setPos(_projectiles[i].getPos() - _projectiles[i].getMotionVec());
				_projectiles[j].setPos(_projectiles[j].getPos() - _projectiles[j].getMotionVec());
			} else {
				j++;
			}
		}*/

		if (!_projectiles[i].update()) {
			_projectiles[i] = _projectiles.back();
			_projectiles.pop_back();
		} else {
			i++;
		}
	}
}

void MainGame::renderGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorShaderProgram.bindShaderProgram();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorShaderProgram.getUniformLocation("textureSampler");
	glUniform1i(textureLocation, 0);

	GLint cameraOrthoProjLocation = _colorShaderProgram.getUniformLocation("orthoProjectionMatrix");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(cameraOrthoProjLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();

	//Load textures
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	
	ViXeL::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 pos(0.0f, 0.0f, 0.0f, 0.0f);

	for (auto& it : _loadedTextures) {
		ViXeL::GLTexture texture = ViXeL::ResourceManager::getTexture(it);
		pos *= glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
		pos += glm::vec4(0.0f, 0.0f, texture.width, texture.height);
		_spriteBatch.render(pos, uv, texture.id, 0.0f, color);
		pos += glm::vec4(texture.width + 5.0f, 0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < _projectiles.size(); ++i) {
		_projectiles[i].render(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_colorShaderProgram.unbindShaderProgram();

	_window.swapBuffer();
}