#include "MultiplayerGameState.h"
#include "MultiplayerConnectState.h"

#include <PYRO/StateStack.h>

#include <SFML/System/Sleep.hpp>
#include <SFML/Network/TcpListener.hpp>

MultiplayerGameState::MultiplayerGameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: GameState(stack, window)
	, thread_(&MultiplayerGameState::packetHandling, this)
	, opponent_port_(0)
{
	mMusicPlayer.pause(true);

	mBaseOpponent = std::unique_ptr<Base>(new Base(Entity::Side::Enemy, mWorldBounds, mDisplayDamageFont,
		                                           mBaseTexture, mUnitTextures, mTurretTextures, mSoundPlayer));

	auto* connectState = const_cast<MultiplayerConnectState*>(dynamic_cast<const MultiplayerConnectState*>(stack_->getState(pyro::StateID::MultiplayerConnect)));
	if (connectState) {
		host_ = connectState->getHostIP().empty() || connectState->getHostPort().empty();

		if (!host_) {
			opponent_ip_ = connectState->getHostIP();
			opponent_port_ = static_cast<sf::Uint16>(std::stoi(connectState->getHostPort()));
		}
	}

	thread_.launch();
}

MultiplayerGameState::~MultiplayerGameState()
{
	thread_.terminate();
}

void MultiplayerGameState::packetHandling()
{
	if (host_) {
		sf::TcpListener listener;
		if (listener.listen(53000) == sf::Socket::Status::Done)
			listener.accept(socket_);
	}
	else if (socket_.connect(opponent_ip_, opponent_port_) != sf::Socket::Done) {
		requestStateClear();
		return;
	}

	//mStack.removeState(pyro::StateID::MultiplayerConnect);
	mMusicPlayer.pause(false);
		
	while (true) {
		if (host_) {
			// RECEIVE
			sf::Packet	  client_packet;
			if (socket_.receive(client_packet) == sf::Socket::Done)
				assert(client_packet >> *mBaseOpponent);

			// SEND
			sf::Packet hostPacket;
			assert(hostPacket << *mBasePlayer);
			socket_.send(hostPacket);
		}
		else {
			// SEND
			sf::Packet client_packet;
			assert(client_packet << *mBasePlayer);
			socket_.send(client_packet);

			// RECEIVE
			sf::Packet host_packet;
			socket_.receive(host_packet);
			assert(host_packet >> *mBaseOpponent);
		}

		sf::sleep(sf::milliseconds(10));
	}
}

bool MultiplayerGameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		requestStateClear();
	}

	if (socket_.getRemoteAddress() != sf::IpAddress::None) {
		mBasePlayer->handleEvent(event);
		return true;
	}

	return false;
}

bool MultiplayerGameState::update(sf::Time dt)
{
	if (socket_.getRemoteAddress() != sf::IpAddress::None)
		GameState::update(dt);

	return false;
}

void MultiplayerGameState::draw()
{
	if (socket_.getRemoteAddress() != sf::IpAddress::None)
		GameState::draw();
}