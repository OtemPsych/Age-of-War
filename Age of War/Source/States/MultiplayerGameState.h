#ifndef MultiplayerGameState_H_
#define MultiplayerGameState_H_

#include "GameState.h"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

class MultiplayerGameState : public GameState
{
public:
	MultiplayerGameState(pyro::StateStack* stack, sf::RenderWindow* window);
	~MultiplayerGameState();
public:
	void packetHandling();
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;

private:
	sf::IpAddress opponent_ip_;
	sf::Uint16    opponent_port_;
	bool		  host_;

	sf::TcpSocket socket_;
	sf::Thread    thread_;
};
#endif