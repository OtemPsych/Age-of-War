#include "MultiplayerConnectState.h"

#include <PYRO/Utils.h>

#include <iostream>

MultiplayerConnectState::MultiplayerConnectState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mIpTextbox(window, sf::Vector2f(250.f, 30.f))
	, mPortTextbox(window, sf::Vector2f(125.f, 30.f))
	, mConnectButton(window, sf::Vector2f(125.f, 30.f))
	, mBackButton(window, sf::Vector2f(125.f, 30.f))
{
	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);

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

	sf::RectangleShape& ipBox(mIpTextbox.getBox());
	pyro::Text& ipText(mIpTextbox.getText());
	ipBox.setFillColor(sf::Color(143, 177, 189, 150));
	ipText.setFont(mFont);
	ipText.setShadowColor(sf::Color::Transparent);
	ipText.setOriginFlags(pyro::utils::OriginFlags::Center);
	ipText.setPosition(ipBox.getSize() / 2.f);
	mIpTextbox.setOriginFlags(pyro::utils::OriginFlags::Center);
	mIpTextbox.setPosition(winSize.x / 2.f, winSize.y / 2.f);

	sf::RectangleShape& portBox(mPortTextbox.getBox());
	pyro::Text& portText(mPortTextbox.getText());
	portBox.setFillColor(ipBox.getFillColor());
	portText.setFont(mFont);
	portText.setShadowColor(ipText.getShadowColor());
	portText.setOriginFlags(pyro::utils::OriginFlags::Center);
	portText.setPosition(portBox.getSize() / 2.f);
	mPortTextbox.setOriginFlags(pyro::utils::OriginFlags::Center);
	mPortTextbox.setPosition(mIpTextbox.getPosition().x, mIpTextbox.getPosition().y + mIpTextbox.getGlobalBounds().height * 1.5f);

	sf::RectangleShape& connectBox(mConnectButton.getBox());
	pyro::Text& connectText(mConnectButton.getText());
	connectBox.setFillColor(ipBox.getFillColor());
	connectText.setFont(mFont);
	connectText.setString("Connect");
	connectText.setShadowColor(ipText.getShadowColor());
	connectText.setOriginFlags(pyro::utils::OriginFlags::Center);
	connectText.setPosition(connectBox.getSize() / 2.f);
	mConnectButton.setOriginFlags(pyro::utils::OriginFlags::Center);
	mConnectButton.setPosition(mPortTextbox.getPosition().x, mPortTextbox.getPosition().y + mPortTextbox.getGlobalBounds().height * 3.f);

	sf::RectangleShape& backBox(mBackButton.getBox());
	pyro::Text& backText(mBackButton.getText());
	backBox.setFillColor(sf::Color(255, 255, 255, 50));
	backText.setFont(mFont);
	backText.setString("< Back");
	backText.setShadowColor(ipText.getShadowColor());
	backText.setOriginFlags(pyro::utils::OriginFlags::Center);
	backText.setPosition(backBox.getSize() / 2.f);
	mBackButton.setOriginFlags(pyro::utils::OriginFlags::Center);
	sf::FloatRect backGBounds(mBackButton.getGlobalBounds());
	mBackButton.setPosition(backGBounds.width / 2.f + 15.f, winSize.y - backGBounds.height / 2.f - 15.f);
}

void MultiplayerConnectState::setupResources()
{
	mBackgroundTexture.loadFromFile("Assets/Textures/MenuBackground.jpg");

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");
}

bool MultiplayerConnectState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed) {
		requestStateClear();
		return false;
	}

	if (mBackButton.clicked(event)) {
		requestStateClear();
		requestStatePush(pyro::StateID::Menu);
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

bool MultiplayerConnectState::update(sf::Time dt)
{
	if (mConnectButton.hover())
		mConnectButton.getBox().setFillColor(sf::Color(255, 255, 255, 120));
	else if (mBackButton.hover())
		mBackButton.getBox().setFillColor(sf::Color(255, 255, 255, 120));
	else {
		mConnectButton.getBox().setFillColor(sf::Color(143, 177, 189, 150));
		mBackButton.getBox().setFillColor(sf::Color(255, 255, 255, 50));
	}

	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));

	return true;
}

void MultiplayerConnectState::draw()
{
	mWindow.draw(mBackgroundSprite);
	mWindow.draw(mIpTextbox);
	mWindow.draw(mPortTextbox);
	mWindow.draw(mConnectButton);
	mWindow.draw(mBackButton);

	mWindow.draw(mCursor);
}

std::string MultiplayerConnectState::getHostIP()
{
	return mIpTextbox.getText().getString();
}

std::string MultiplayerConnectState::getHostPort()
{
	return mPortTextbox.getText().getString();
}