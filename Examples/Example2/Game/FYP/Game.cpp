#include "stdafx.h"
#include "../../../../MetaSystem/MetaSystem.h"
#include <iostream>
using namespace std;

#include "LTimer.h"
#include "Game.h"

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Game::Game(Size2D screenSize, Size2D worldSize) : 
	m_camera(new Camera2D(Rect(0, 0, screenSize.w, screenSize.h), 1)), 
	m_screenSize(screenSize), 
	m_worldSize(worldSize)
{
	quit = false;
	m_camera->setLevelSize(Size2D(worldSize.w, worldSize.h));
}

Game::~Game()
{
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}



bool Game::init() {
	Size2D winSize(m_screenSize.w, m_screenSize.h);
	srand(0);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize, "Demo", m_camera);


	//create some game objects
	for (int i = 0; i < 50; i++)
	{
		float radius = rand() % 20 + 5;
		Point2D initPos(rand() % (int)(m_worldSize.w - radius) + radius, rand() % (int)(m_worldSize.h - radius) + radius);
		double randAngle = RandomFloat(0, 2 * M_PI);
		float randSpeed = rand() % 100 + 40;
		Rect worldBounds(Point2D(), m_worldSize);
		Point2D randVel(cos(randAngle), sin(randAngle));

		CircleObject * p = new CircleObject(initPos, randVel * randSpeed, radius);
		p->SetBounds(worldBounds); 

		gameObjects.push_back(p);
	}

	//add some game objects
	
	lastTime = LTimer::gameTime();

	//inputManager.AddListener(EventListener::Event::EXAMPLE, EventListener);
	inputManager.AddListener(EventListener::Event::QUIT, this);
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
void Game::update(vector<GameObject*>& objects)
{
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	float deltaTime = (currentTime - lastTime) / 1000.f;//time since last update

	//call update on all game objects
	for (std::vector<GameObject*>::iterator i = objects.begin(); i != objects.end(); i++) 
	{
		(*i)->Update(deltaTime);
	}

	//save the curent time for next frame
	lastTime = currentTime;
}

//** calls render on all game entities*/

void Game::render(vector<GameObject*>& objects)
{
	renderer.clear(Colour(0,0,0));// prepare for new frame
	Rect camPos = (m_camera->getViewport() / m_camera->getScale());
	//render every object
	for (std::vector<GameObject*>::iterator i = objects.begin(), e= objects.end(); i != e; i++) 
	{
		if (camPos.containsPoint((*i)->getTopLeft()))
		{
			(*i)->Render(renderer);
		}
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

		update(gameObjects);
		render(gameObjects);

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
	default:
		break;
	}

}
