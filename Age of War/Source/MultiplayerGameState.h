#ifndef MultiplayerGameState_H_
#define MultiplayerGameState_H_

#include "GameState.h"

#include <SFML/System/Thread.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

class MultiplayerGameState : public GameState
{
private:
	sf::IpAddress mOpponentIP;
	sf::Uint16    mOpponentPort;
	bool		  mHost;

	sf::TcpSocket mSocket;
	sf::Thread    mThread;

public:
	MultiplayerGameState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	void packetHandling();
};
#endif