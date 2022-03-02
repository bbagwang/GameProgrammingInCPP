#include "Game.h"

int main( int argc, char** argv )
{
	Game PongGame;
	
	bool bSuccess = PongGame.Initialize();
	if (bSuccess)
	{
		PongGame.RunLoop();
	}

	PongGame.Shutdown();

	return 0;
}