#include "headers/game.h"

int main()
{
	Game game("Learnin' Opengl",
		480, 480,
		4, 4,
		true);
		
	game.saveDepthMap();
	//MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		game.update();
		game.render();	
	}

	return 0;
}