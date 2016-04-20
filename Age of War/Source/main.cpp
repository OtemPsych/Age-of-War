#include "GameState.h"

#include <PYRO/Application.h>

int main()
{
	srand(static_cast<unsigned>(time(0)));

	pyro::Application app(sf::VideoMode::getDesktopMode(), "Age of War",
						  sf::Style::Fullscreen);
	app.setClearScreen(true);
	app.setFPS(144);

	app.registerState<GameState>(pyro::StateID::Game);
	app.pushState(pyro::StateID::Game);
	app.run();

	return 0;
}