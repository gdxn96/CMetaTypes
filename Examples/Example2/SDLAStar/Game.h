#pragma once

#include <vector>

#include "Renderer.h"
#include "GameObject.h"
#include "InputManager.h"
#include "EventListener.h"
#include "Camera2D.h"


/** The game objct whic manages the game loop*/
class Game:public EventListener
{
	InputManager inputManager;
	Renderer renderer;

	std::vector<GameObject*> gameObjects;

	unsigned int lastTime;//time of last update;

	bool pause;
	bool quit;

	Camera2D * m_camera;
	Size2D m_screenSize;
	Size2D m_worldSize;

public:
	Game(Size2D screenSize, Size2D worldSize);
	~Game();

	bool init();
	void destroy();

	void update();
	void render();
	void loop();

	void onEvent(EventListener::Event);
};

