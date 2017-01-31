//  Defines the entry point for the console application.
//
#include "stdafx.h"

#include <iostream>
using namespace std;
#include "Game.h"


/**Create and initialises an instance of game, and clean up when the game is closed*/

int main()
{
	srand(0);
	Game game(Size2D(1080, 1080), Size2D(1080, 1080));

	if (!game.init()) {
		cout << "Failed to init game"<<'\n';
	}

	//run the game loop
	game.loop();

	game.destroy();

    return 0;
}

