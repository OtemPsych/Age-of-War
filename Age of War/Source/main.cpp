#include "States/MenuState.h"
#include "States/MultiplayerConnectState.h"
#include "States/MultiplayerGameState.h"
#include "States/PauseState.h"

#include <PYRO/Application.h>

int main()
{
	srand(static_cast<unsigned>(time(0)));

	pyro::Application app(sf::VideoMode(1280, 720), "Age of War", 
		sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));
	app.setFPS(144);
	app.registerState<MenuState>(pyro::StateID::Menu);
	app.registerState<MultiplayerConnectState>(pyro::StateID::MultiplayerConnect);
	app.registerState<MultiplayerGameState>(pyro::StateID::Multiplayer);
	app.registerState<GameState>(pyro::StateID::Game);
	app.registerState<PauseState>(pyro::StateID::Pause);
	app.pushState(pyro::StateID::Menu);
	app.run();

	return 0;
}