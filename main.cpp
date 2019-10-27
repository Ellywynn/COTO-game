#include "Game.h"
#include <ctime>

int main()
{
	std::srand(static_cast<unsigned int>(time(0)));

	Game game;

	while (game.running())
	{
		game.update();
		game.render();
	}
	return 0;
}