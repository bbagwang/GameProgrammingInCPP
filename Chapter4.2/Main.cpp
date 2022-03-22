#include "Game.h"

//#include "Search.h"

int main(int argc, char** argv)
{
	Game game;

	if (game.Initialize())
	{
		game.RunLoop();
	}

	game.Shutdown();
	
	//testTicTac();

	return 0;
}
