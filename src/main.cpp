#include "headers/game.h"

int main()
{
	Game game("Learnin' Opengl",
		1600, 900,
		4, 4,
		true);
	// game.saveDepthMap();
	//MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		game.update();
		game.render();	
	}

	return 0;
}