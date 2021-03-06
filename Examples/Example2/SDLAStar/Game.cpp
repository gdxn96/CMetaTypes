#include "stdafx.h"
#include <iostream>
using namespace std;

#include "LTimer.h"
#include "Game.h"
#include "SpinningBox.h"


const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game(Size2D screenSize, Size2D worldSize) : m_camera(new Camera2D(Rect(0, 0, screenSize.w, screenSize.h), 1)), m_screenSize(screenSize), m_worldSize(worldSize)
{
	quit = false;
	m_camera->setLevelSize(Size2D(worldSize.w, worldSize.h));
	gameObjects.push_back(new SpinningBox(Rect(rand() % 400 + 400, rand() % 400 + 400, 50, 50)));

	auto& list = EntityList::get<SpinningBox>();
}


Game::~Game()
{
}


bool Game::init() {	
	Size2D winSize(m_screenSize.w, m_screenSize.h);
	srand(0);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize,"A* Threading", m_camera);


	//create some game objects


	//add some game objects
	
	lastTime = LTimer::gameTime();

	//inputManager.AddListener(EventListener::Event::EXAMPLE, EventListener);

	inputManager.AddListener(EventListener::Event::QUIT, this);
	inputManager.AddListener(EventListener::Event::UP, this);
	inputManager.AddListener(EventListener::Event::DOWN, this);
	inputManager.AddListener(EventListener::Event::LEFT, this);
	inputManager.AddListener(EventListener::Event::RIGHT, this);
	inputManager.AddListener(EventListener::Event::ZOOM_IN, this);
	inputManager.AddListener(EventListener::Event::ZOOM_OUT, this);
	return true;
}


void Game::destroy()
{
	//empty out the game object vector before quitting
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(); i != gameObjects.end(); i++) {
		delete *i;
	}
	gameObjects.clear();
	renderer.destroy();
}

//** calls update on all game entities*/
void Game::update()
{
	EntityList::get<SpinningBox>();
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	float deltaTime = (currentTime - lastTime) / 1000.0;//time since last update

	//need to call update on fileMonitor in update
	FileMonitor::getInstance().MonitorFiles(deltaTime);

	//call update on all game objects
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(); i != gameObjects.end(); i++) {
		(*i)->Update(deltaTime);
	}

	//save the curent time for next frame
	lastTime = currentTime;
}

//** calls render on all game entities*/

void Game::render()
{
	renderer.clear(Colour(0,0,0));// prepare for new frame
	
	//render every object
	for (std::vector<GameObject*>::iterator i = gameObjects.begin(), e= gameObjects.end(); i != e; i++) {
		(*i)->Render(renderer);
	}

	renderer.present();// display the new frame (swap buffers)
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;//to cap framerate

	int frameNum = 0;
	while (!quit) { //game loop
		capTimer.start();

		inputManager.ProcessInput();
		update();
		render();

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}

void Game::onEvent(EventListener::Event evt) {
	switch (evt)
	{
	case(EventListener::Event::QUIT):
		quit = true;
		break;
	case(EventListener::Event::LEFT):
		m_camera->MoveLeft();
		break;
	case(EventListener::Event::RIGHT):
		m_camera->MoveRight();
		break;
	case(EventListener::Event::UP):
		m_camera->MoveUp();
		break;
	case(EventListener::Event::DOWN):
		m_camera->MoveDown();
		break;
	case(EventListener::Event::ZOOM_IN):
		m_camera->increaseScale();
		break;
	case(EventListener::Event::ZOOM_OUT):
		m_camera->decreaseScale();
		break;
	default:
		break;
	}

}
