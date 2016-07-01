#include "MultiplayerGameState.h"
#include "MultiplayerConnectState.h"

#include <PYRO/StateStack.h>

#include <SFML/System/Sleep.hpp>
#include <SFML/Network/TcpListener.hpp>

MultiplayerGameState::MultiplayerGameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: GameState(stack, window)
	, mThread(&MultiplayerGameState::packetHandling, this)
	, mOpponentPort(0)
{
	mMusicPlayer.pause(true);

	sf::Vector2u winSize(mWindow.getSize());
	mBaseOpponent = std::unique_ptr<Base>(new Base(Entity::Side::Enemy, mWorldBounds, mDisplayDamageFont, mBaseTexture, mUnitTextures,
		                                           mUnitData, mTurretTextures, mTurretData, mSoundPlayer));

	auto* connectState = const_cast<MultiplayerConnectState*>(dynamic_cast<const MultiplayerConnectState*>(stack.getState(pyro::StateID::MultiplayerConnect)));
	if (connectState) {
		mHost = connectState->getHostIP().empty() || connectState->getHostPort().empty();

		if (!mHost) {
			mOpponentIP = connectState->getHostIP();
			mOpponentPort = static_cast<sf::Uint16>(std::stoi(connectState->getHostPort()));
		}
	}

	mThread.launch();
}

MultiplayerGameState::~MultiplayerGameState()
{
	mThread.terminate();
}

void MultiplayerGameState::packetHandling()
{
	if (mHost) {
		sf::TcpListener listener;
		if (listener.listen(53000) == sf::Socket::Status::Done)
			listener.accept(mSocket);
	}
	else if (mSocket.connect(mOpponentIP, mOpponentPort) != sf::Socket::Done) {
		requestStateClear();
		return;
	}

	//mStack.removeState(pyro::StateID::MultiplayerConnect);
	mMusicPlayer.pause(false);
		
	while (true) {
		if (mHost) {
			// RECEIVE
			sf::Packet	  clientPacket;
			if (mSocket.receive(clientPacket) == sf::Socket::Done)
				assert(clientPacket >> *mBaseOpponent);

			// SEND
			sf::Packet hostPacket;
			assert(hostPacket << *mBasePlayer);
			mSocket.send(hostPacket);
		}
		else {
			// SEND
			sf::Packet clientPacket;
			assert(clientPacket << *mBasePlayer);
			mSocket.send(clientPacket);

			// RECEIVE
			sf::Packet hostPacket;
			mSocket.receive(hostPacket);
			assert(hostPacket >> *mBaseOpponent);
		}

		sf::sleep(sf::milliseconds(10));
	}
}

bool MultiplayerGameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		requestStateClear();
	}

	if (mSocket.getRemoteAddress() != sf::IpAddress::None) {
		mBasePlayer->handleEvent(event);
		return true;
	}

	return false;
}

bool MultiplayerGameState::update(sf::Time dt)
{
	if (mSocket.getRemoteAddress() != sf::IpAddress::None)
		GameState::update(dt);

	return false;
}

void MultiplayerGameState::draw()
{
	if (mSocket.getRemoteAddress() != sf::IpAddress::None)
		GameState::draw();
}