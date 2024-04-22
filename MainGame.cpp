#include "MainGame.h"

#include <ViXeL/ViXeL.h>
#include <ViXeL/data/ResourceManager.h>
#include <ViXeL/data/Vertex.h>
#include <ViXeL/error/Errors.h>
#include <ViXeL/event/EventManager.h>
#include <glm/ext/matrix_transform.hpp>

#include "AutoLoader.h"
#include "EventRegistration.h"

#include <typeinfo>

GameState MainGame::gameState = GameState::RUNNING;

// world space positions of our cubes
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

MainGame::MainGame() :
    _windowWidth(1024),
    _windowHeight(768),
    _testCamera2d(ViXeL::Camera<ViXeL::Camera2D>(_windowWidth, _windowHeight, {0.0f, 0.0f}, 1.0f)),
    _testCamera3d(ViXeL::Camera<ViXeL::PerspectiveCamera3D>(_windowWidth, _windowHeight, {0.0f, 0.0f, 0.0f}, 1.0f, 45.0f, { -90.0f, 0.0f },  0.1f)),
    _playerPos(glm::vec2(0.0f, 0.0f)),
    _maxFps(165.0f),
    _vsync(1),
    _fpsLimiter(_maxFps),
    _gameTime(0.0f) {

    registerEvents();

}

MainGame::~MainGame() = default;

void MainGame::run() {
	init();
	
	gameLoop();
}

void MainGame::init() {
    ViXeL::init();

    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    _window.create("GeoPixel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL);

    std::cout << glGetString(GL_VENDOR) << '\n';
    std::cout << glGetString(GL_RENDERER) << '\n';
    std::cout << glGetString(GL_VERSION) << '\n';

    //Set VSYNC
    SDL_GL_SetSwapInterval(_vsync);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    _window.grabMouse();

    initShaders();

    cubeMesh.create();

    _spriteBatch.init();

    fetchSprites(&_loadedTextures);

    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_KEYDOWN), &keyPressed);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEMOTION), &mouseMoved);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEWHEEL), &scrolled);
    ViXeL::EventManager::getInstance().listenForEvent(std::to_string(SDL_MOUSEBUTTONDOWN), &mouseButtonPressed);


    _cameras.push_back({_testCamera2d});
}

void MainGame::initShaders() {
    _colorShaderProgram.compileShaders("resources/Shaders/colorShader.vert", "resources/Shaders/colorShader.frag");
	_colorShaderProgram.addAttribute("vertexPosition");
	_colorShaderProgram.addAttribute("vertexColor");
	_colorShaderProgram.addAttribute("vertexUV");
	_colorShaderProgram.linkShaders();

    _testShaderProgram.compileShaders("resources/Shaders/testShader.vert", "resources/Shaders/testShader.frag");
    _testShaderProgram.addAttribute("vertexPosition");
    _testShaderProgram.addAttribute("vertexColor");
    _testShaderProgram.addAttribute("vertexUV");
    _testShaderProgram.linkShaders();


    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    /*glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ViXeL::GLTexture watermellonCatTexture = ViXeL::ResourceManager::getTexture(std::filesystem::path("resources/Textures/2_watermelon-cat.png"));
    glBindTexture(GL_TEXTURE_2D, watermellonCatTexture.id);

    glBindVertexArray(0);*/

    cubeMesh.setOrigin({0.0f, 0.0f, 0.0f});

    for(int i = 0; i < 36; i++) {
        glm::vec3 vertex = glm::vec3(vertices[i*5], vertices[i*5 + 1], vertices[i*5 + 2]);
        glm::vec2 uv = glm::vec2(vertices[i*5 + 3], vertices[i*5 + 4]);
        cubeMesh.addVertex(ViXeL::ColoredTexturedVertex3D({vertex.x, vertex.y, vertex.z}, {255, 255, 255, 255}, {uv.x, uv.y}));
    }

    //cubeMesh.setTexture(ViXeL::ResourceManager::getTexture(std::filesystem::path("resources/Textures/1_catEatWatermellonBecomeWatermellon.png")));

    cubePositions.push_back(cubeMesh.origin);
    //cubePositions.push_back({10.0f, 0.0f, 0.0f});
    //cubePositions.push_back({8.0f, 0.0f, 0.0f});

    //cube3 = cubeMesh;

    // C U B E
}

void MainGame::gameLoop() {
	while (gameState != GameState::EXIT) {
	    _fpsLimiter.begin();
		
	    processEvents();

	    _gameTime += 0.01;

	    

	    /*constexpr float radius = 10.0f;
	    float rotX = cos(_gameTime) * radius;
	    float rotZ = sin(_gameTime) * radius;

	    _testCamera3d.setPos({rotX, 10.0f, rotZ});
	    _testCamera3d.setRotationRadians(atan2(rotZ, rotX) - std::numbers::pi, glm::radians(-45.0f));*/

		_testCamera2d.update();
	    _testCamera3d.update();

		processPhysics();

	    //ViXeL::EventManager::getInstance().triggerEvent("RENDER");
		renderGame();

		//static int frameCounter = 0;
		//frameCounter++;
		/*if (frameCounter == 10) {
			std::cout << "Current fps: " << _fps << "\r";
			frameCounter = 0;
		}*/

	    if(_window.isMouseGrabbed()) {
	        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	        _window.setMousePosition(_windowWidth / 2, _windowHeight / 2);
	        SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
	    }

		_fps = _fpsLimiter.endFrame();
	}
}

void MainGame::processEvents() {
    ViXeL::EventManager::getInstance().parse();

    constexpr bool CAMERA_PERSPECTIVE = false;

    constexpr float CAMERA_PAN_SPEED = 0.01f;
    constexpr float CAMERA_ROTATE_SPEED = 0.05f;
    constexpr float CAMERA_ZOOM_SPEED = 0.45f;

    if (keyPressed) {
        if (ViXeL::InputManager::getKeyState(SDLK_w) || ViXeL::InputManager::getKeyState(SDLK_a) || ViXeL::InputManager::getKeyState(SDLK_s) || ViXeL::InputManager::getKeyState(SDLK_d)) {
            glm::vec3 cameraPosModifier = glm::vec3(0.0f, 0.0f, 0.0f);
            if (ViXeL::InputManager::getKeyState(SDLK_w)) {
                cameraPosModifier = cameraPosModifier + glm::normalize(glm::vec3(_testCamera3d.getCameraFacing().x, 0.0f, _testCamera3d.getCameraFacing().z));
                _testCamera2d.setPos(_testCamera2d.getPos() + glm::vec2(0.0f, CAMERA_PAN_SPEED));
            }

            if (ViXeL::InputManager::getKeyState(SDLK_a)) {
                cameraPosModifier = cameraPosModifier - _testCamera3d.getCameraRight();
                _testCamera2d.setPos(_testCamera2d.getPos() + glm::vec2(-CAMERA_PAN_SPEED, 0.0f));
            }

            if (ViXeL::InputManager::getKeyState(SDLK_s)) {
                cameraPosModifier = cameraPosModifier - glm::normalize(glm::vec3(_testCamera3d.getCameraFacing().x, 0.0f, _testCamera3d.getCameraFacing().z));
                _testCamera2d.setPos(_testCamera2d.getPos() + glm::vec2(0.0f, -CAMERA_PAN_SPEED));
            }

            if (ViXeL::InputManager::getKeyState(SDLK_d)) {
                cameraPosModifier = cameraPosModifier + _testCamera3d.getCameraRight();
                _testCamera2d.setPos(_testCamera2d.getPos() + glm::vec2(CAMERA_PAN_SPEED, 0.0f));
            }

            if (cameraPosModifier != glm::vec3(0.0f, 0.0f, 0.0f)) {
                _testCamera3d.setPos(_testCamera3d.getPos() + glm::normalize(cameraPosModifier) * CAMERA_PAN_SPEED);
            }
        }

        if (ViXeL::InputManager::getKeyState(SDLK_e)) { _testCamera3d.setPos(_testCamera3d.getPos() + glm::vec3(0.0f, CAMERA_PAN_SPEED, 0.0f)); }

        if (ViXeL::InputManager::getKeyState(SDLK_q)) { _testCamera3d.setPos(_testCamera3d.getPos() + glm::vec3(0.0f, -CAMERA_PAN_SPEED, 0.0f)); }


        if (ViXeL::InputManager::getKeyState(SDLK_LALT)) {
            _window.ungrabMouse();
        } else if (!_window.isMouseGrabbed()) {
            _window.grabMouse();
        }
    }

    if (mouseMoved && !ViXeL::InputManager::getKeyState(SDLK_LALT)) {
        if (_testCamera3d.getRotationDegrees().x != -89.99f && _testCamera3d.getRotationDegrees().x != 89.99f) {
            float mouseVertical = _testCamera3d.getRotationDegrees().y - (ViXeL::InputManager::getMousePos().y * CAMERA_ROTATE_SPEED);
            if (mouseVertical > -89.99f && mouseVertical < 89.99f) {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    mouseVertical
                );
            } else if (mouseVertical > 89.99f) {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    89.99f
                );
            } else {
                _testCamera3d.setRotationDegrees(
                    _testCamera3d.getRotationDegrees().x,
                    -89.99f
                );
            }
        }
        
        _testCamera3d.setRotationDegrees(_testCamera3d.getRotationDegrees() + glm::vec2(
            ViXeL::InputManager::getMousePos().x * CAMERA_ROTATE_SPEED,
            0.0f
        ));

        mouseMoved = false;
    }

    if (CAMERA_PERSPECTIVE) {
        if (scrolled) {
            if (_testCamera3d.getFovDegrees() < 89.95f) {
                float zoomAmount = _testCamera3d.getFovDegrees() - (ViXeL::InputManager::getScrollDirection().y * CAMERA_ZOOM_SPEED);
                if(zoomAmount > 15.0f && zoomAmount < 89.95f) {
                    _testCamera3d.setFovDegrees(zoomAmount);
                } else if (zoomAmount < 15.0f) {
                    _testCamera3d.setFovDegrees(15.0f);
                }
            } else if (_testCamera3d.getFovDegrees() > 15.0f) {
                float zoomAmount = _testCamera3d.getFovDegrees() - (ViXeL::InputManager::getScrollDirection().y * CAMERA_ZOOM_SPEED);
                if(zoomAmount > 15.0f && zoomAmount < 89.95f) {
                    _testCamera3d.setFovDegrees(zoomAmount);
                } else if (zoomAmount > 89.95f) {
                    _testCamera3d.setFovDegrees(89.95f);
                }
            }
            scrolled = false;
        }
    }

    if (mouseButtonPressed) {
        if (ViXeL::InputManager::getKeyState(SDL_BUTTON_LEFT)) {
            glm::vec2 mousePos = ViXeL::InputManager::getMousePos();
            mousePos = _testCamera2d.toWorldPos(mousePos);

            glm::vec2 direction = mousePos - _playerPos;
            direction = glm::normalize(direction);

            _projectiles.emplace_back(_playerPos, direction, 0.125f, 1000, glm::vec4(-15.0f, -15.0f, 15.0f, 15.0f));
        }

        if (ViXeL::InputManager::getKeyState(SDL_BUTTON_RIGHT)) {
            _playerPos = _testCamera2d.toWorldPos(ViXeL::InputManager::getMousePos());
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
	//glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(render2dEnabled) {
        //Begin 2D rendering
        _colorShaderProgram.bind();

        glActiveTexture(GL_TEXTURE0);

        GLint textureLocation = _colorShaderProgram.getUniformLocation("textureSampler");
        glUniform1i(textureLocation, 0);

        GLint cameraOrthoProjLocation = _colorShaderProgram.getUniformLocation("orthoProjectionMatrix");
        glm::mat4 cameraMatrix = _testCamera2d.getCameraMatrix();

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
        _colorShaderProgram.unbind();
        //End 2D rendering
    }

    //Begin 3D rendering
    _testShaderProgram.bind();
    glActiveTexture(GL_TEXTURE0);

    ViXeL::GLTexture watermellonCatTexture = ViXeL::ResourceManager::getTexture(std::filesystem::path("resources/Textures/catEatWatermellonBecomeWatermellon.png"));
    glBindTexture(GL_TEXTURE_2D, watermellonCatTexture.id);

    /*GLint isTexturedLocation = _testShaderProgram.getUniformLocation("isTextured");
    if(cubeMesh.isTextured()) {
        glUniform1i(isTexturedLocation, 1);
        GLint textureLocation = _testShaderProgram.getUniformLocation("textureSampler");
        glUniform1i(textureLocation, 0);
    } else {
        glUniform1i(isTexturedLocation, 0);
    }*/

    GLint textureLocation = _testShaderProgram.getUniformLocation("textureSampler");
    glUniform1i(textureLocation, 0);

    GLint cameraMatrixLocation = _testShaderProgram.getUniformLocation("cameraMatrix");
    glm::mat4 cameraMatrix = _testCamera3d.getCameraMatrix();
    glUniformMatrix4fv(cameraMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

    for(auto it : cubePositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), it);
        GLint modelLocation = _testShaderProgram.getUniformLocation("model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &(model[0][0]));

        cubeMesh.render();
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    _testShaderProgram.unbind();
    //End 3D rendering

	_window.swapBuffer();
}
