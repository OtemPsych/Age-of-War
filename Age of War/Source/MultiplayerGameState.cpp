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
	sf::Vector2u winSize(mWindow.getSize());
	mBaseOpponent = std::unique_ptr<Base>(new Base(Entity::Side::Right, sf::IntRect(0, 0, winSize.x, winSize.y),
												   mBaseTexture, mUnitTextures, mUnitData, mSoundPlayer));

	auto* connectState = const_cast<MultiplayerConnectState*>(dynamic_cast<const MultiplayerConnectState*>(stack.getState(pyro::StateID::MultiplayerConnect)));
	if (connectState) 
	{
		mHost = connectState->getHostIP().empty() || connectState->getHostPort().empty();

		if (!mHost) 
		{
			mOpponentIP = connectState->getHostIP();
			mOpponentPort = static_cast<sf::Uint16>(std::stoi(connectState->getHostPort()));
		}
	}

	stack.removeState(pyro::StateID::MultiplayerConnect);

	if (mHost)
	{
		sf::TcpListener listener;
		if (listener.listen(53000) == sf::Socket::Status::Done)
			listener.accept(mSocket);
	}
	else {
		if (mSocket.connect(mOpponentIP, mOpponentPort, sf::seconds(3.f)) != sf::Socket::Done)
		{
			requestStateClear();
			return;
		}
	}

	mThread.launch();
}

void MultiplayerGameState::packetHandling()
{
	while (mPlaying)
	{
		if (mHost)
		{
			// RECEIVE
			sf::Packet	  clientPacket;
			if (mSocket.receive(clientPacket) == sf::Socket::Done)
				assert(clientPacket >> *mBaseOpponent);

			// SEND
			sf::Packet hostPacket;
			assert(hostPacket << *mBasePlayer);
			mSocket.send(hostPacket);
		}
		else
		{
			// SEND
			sf::Packet clientPacket;
			assert(clientPacket << *mBasePlayer);
			mSocket.send(clientPacket);

			// RECEIVE
			sf::Packet hostPacket;
			mSocket.receive(hostPacket);
			assert(hostPacket >> *mBaseOpponent);
		}

		sf::sleep(sf::milliseconds(40));
	}
}