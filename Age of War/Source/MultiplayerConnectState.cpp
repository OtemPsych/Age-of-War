#include "MultiplayerConnectState.h"

MultiplayerConnectState::MultiplayerConnectState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mIpTextbox(window, sf::Vector2f(250.f, 30.f))
	, mPortTextbox(window, sf::Vector2f(125.f, 30.f))
	, mConnectButton(window, sf::Vector2f(125.f, 30.f))
{
	setupResources();

	mBackgroundSprite.setTexture(mBackgroundTexture);

	sf::Vector2f winSize(mWindow.getSize());
	sf::Vector2f textureSize(mBackgroundTexture.getSize());
	mBackgroundSprite.scale(winSize.x / textureSize.x, winSize.y / textureSize.y);

	setupGUIEntities();
}

void MultiplayerConnectState::setupGUIEntities()
{
	sf::Vector2f winSize(mWindow.getSize());
	mIpTextbox.setPosition(winSize.x / 2.f, winSize.y / 2.f);
	mIpTextbox.getShape().setFillColor(sf::Color(143, 177, 189, 150));
	mIpTextbox.getText().setFont(mFont);

	mPortTextbox.setPosition(mIpTextbox.getPosition().x, mIpTextbox.getPosition().y + mIpTextbox.getGlobalBounds().height * 1.5f);
	mPortTextbox.getShape().setFillColor(mIpTextbox.getShape().getFillColor());
	mPortTextbox.getText().setFont(mFont);

	mConnectButton.setPosition(mPortTextbox.getPosition().x, mPortTextbox.getPosition().y + mPortTextbox.getGlobalBounds().height * 3.f);
	mConnectButton.getShape().setFillColor(mIpTextbox.getShape().getFillColor());
	mConnectButton.getText().setFont(mFont);
	mConnectButton.getText().setString("Connect");
}

void MultiplayerConnectState::setupResources()
{
	mBackgroundTexture.loadFromFile("Assets/Textures/MenuBackground.jpg");

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");
}

bool MultiplayerConnectState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		requestStateClear();
		return false;
	}

	mIpTextbox.handleEvent(event);
	mPortTextbox.handleEvent(event);
	if (mConnectButton.clicked(event))
	{
		requestStatePush(pyro::StateID::Multiplayer);
		return false;
	}

	return true;
}


void MultiplayerConnectState::draw()
{
	mWindow.draw(mBackgroundSprite);
	mWindow.draw(mIpTextbox);
	mWindow.draw(mPortTextbox);
	mWindow.draw(mConnectButton);
}

std::string MultiplayerConnectState::getHostIP()
{
	return mIpTextbox.getText().getString();
}

std::string MultiplayerConnectState::getHostPort()
{
	return mPortTextbox.getText().getString();
}