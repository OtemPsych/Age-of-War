#include "MultiplayerConnectState.h"

#include <PYRO/Utils.h>

#include <iostream>

MultiplayerConnectState::MultiplayerConnectState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mIpTextbox(window)
	, mPortTextbox(window)
	, mConnectButton(window, sf::Vector2f(150.f, 45.f))
	, mBackButton(window)
{
	setupResources();

	mCursor.setTexture(mCursorTexture);
	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));
	mCursor.scale(0.9f, 0.9f);

	const sf::Vector2f winSize(mWindow.getSize());
	const sf::Vector2f textureSize(mBackgroundTexture.getSize());
	mBackgroundSprite.setTexture(mBackgroundTexture);
	mBackgroundSprite.scale(winSize.x / textureSize.x, winSize.y / textureSize.y);

	setupGUIEntities();
}

void MultiplayerConnectState::setupGUIEntities()
{
	sf::Vector2f winSize(mWindow.getSize());

	// IP Textbox
	sf::VertexArray& ipVertices = mIpTextbox.getVertices();
	ipVertices.setPrimitiveType(sf::LinesStrip);
	ipVertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(245.f, 0.f),  sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(250.f, 5.f),  sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(250.f, 30.f), sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(245.f, 35.f), sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(5.f,   35.f), sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(0.f,   30.f), sf::Color(158, 187, 236)));
	ipVertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	mIpTextbox.setCaretPosition(sf::Vector2f(250.f, 35.f) / 2.f);
	mIpTextbox.setCaretColor(sf::Color(125, 166, 181));
	mIpTextbox.setPosition(winSize / 2.f - mIpTextbox.getSize() / 2.f);

	mIpTextbox.activateText(true);
	pyro::Text* ipText = mIpTextbox.getText();
	ipText->setFont(mFont);
	ipText->activateShadow(true);
	ipText->setShadowOffset(2.f, 2.f);
	ipText->setShadowColor(sf::Color(0, 0, 0, 80));
	ipText->setOriginFlags(pyro::utils::OriginFlags::Center);
	ipText->setPosition(sf::Vector2f(250.f, 35.f) / 2.f);

	// Port Textbox
	sf::VertexArray& portVertices = mPortTextbox.getVertices();
	portVertices.setPrimitiveType(sf::LinesStrip);
	portVertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(135.f, 0.f),  sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(140.f, 5.f),  sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(140.f, 30.f), sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(135.f, 35.f), sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(5.f,   35.f), sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(0.f,   30.f), sf::Color(158, 187, 236)));
	portVertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	mPortTextbox.setCaretPosition(sf::Vector2f(140.f, 35.f) / 2.f);
	mPortTextbox.setCaretColor(sf::Color(125, 166, 181));
	mPortTextbox.setPosition(winSize.x / 2.f - mPortTextbox.getSize().x / 2.f, mIpTextbox.getPosition().y + mPortTextbox.getGlobalBounds().height * 1.5f);

	mPortTextbox.activateText(true);
	pyro::Text* portText = mPortTextbox.getText();
	portText->setFont(mFont);
	portText->activateShadow(true);
	portText->setTextColor(sf::Color::White);
	portText->setShadowOffset(*ipText->getShadowOffset());
	portText->setShadowColor(*ipText->getShadowColor());
	portText->setOriginFlags(pyro::utils::OriginFlags::Center);
	portText->setPosition(sf::Vector2f(140.f, 35.f) / 2.f);

	// Connect Button
	mConnectButton.setPosition(winSize.x / 2.f - mConnectButton.getSize().x / 2.f, mPortTextbox.getPosition().y + mPortTextbox.getGlobalBounds().height * 2.5f);
	mConnectButtonTexture.loadFromFile("Assets/Textures/MenuButton.png");
	mConnectButton.setTexture(&mConnectButtonTexture);

	mConnectButton.activateText(true);
	pyro::Text* connectText = mConnectButton.getText();
	connectText->setFont(mFont);
	connectText->setString("Connect");
	connectText->activateShadow(true);
	connectText->setShadowOffset(*ipText->getShadowOffset());
	connectText->setShadowColor(*ipText->getShadowColor());
	connectText->setOriginFlags(pyro::utils::OriginFlags::Center);
	connectText->setPosition(mConnectButton.getSize() / 2.f);

	// Back Button
	sf::VertexArray& backVertices = mBackButton.getVertices();
	backVertices.setPrimitiveType(sf::Quads);
	backVertices.append(sf::Vertex(sf::Vector2f(0.f,  15.f), sf::Color::White));
	backVertices.append(sf::Vertex(sf::Vector2f(15.f, 0.f),  sf::Color::White));
	backVertices.append(sf::Vertex(sf::Vector2f(15.f, 15.f), sf::Color::White));
	backVertices.append(sf::Vertex(sf::Vector2f(15.f, 30.f), sf::Color::White));
	backVertices.append(sf::Vertex(sf::Vector2f(15.f, 0.f),  sf::Color::Transparent));
	backVertices.append(sf::Vertex(sf::Vector2f(95.f, 0.f),  sf::Color::Transparent));
	backVertices.append(sf::Vertex(sf::Vector2f(95.f, 30.f), sf::Color::Transparent));
	backVertices.append(sf::Vertex(sf::Vector2f(15.f, 30.f), sf::Color::Transparent));

	mBackButton.activateText(true);
	pyro::Text* backText = mBackButton.getText();
	backText->setFont(mFont);
	backText->setString("Back");
	backText->activateShadow(true);
	backText->setShadowOffset(*ipText->getShadowOffset());
	backText->setShadowColor(*ipText->getShadowColor());
	backText->setOriginFlags(pyro::utils::OriginFlags::Left | pyro::utils::OriginFlags::CenterY);
	backText->setPosition(sf::Vector2f(25.f, 15.f));
	mBackButton.setPosition(15.f + 5.f, winSize.y - mBackButton.getGlobalBounds().height - 15.f);
}

void MultiplayerConnectState::setupResources()
{
	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mBackgroundTexture.loadFromFile("Assets/Textures/MenuBackground.jpg");

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");
}

bool MultiplayerConnectState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));
	}
	else if (event.type == sf::Event::Closed) {
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
	if (mConnectButton.clicked(event)) {
		requestStatePush(pyro::StateID::Multiplayer);
		return false;
	}

	return true;
}

bool MultiplayerConnectState::update(sf::Time dt)
{
	if (mConnectButton.hover()) {
		mConnectButton.setFillColor(sf::Color(155, 255, 155, 255));
	}
	else if (mBackButton.hover()) {
		sf::VertexArray& backVertices = mBackButton.getVertices();
		backVertices[0].color =
		backVertices[1].color =
		backVertices[2].color =
		backVertices[3].color = sf::Color(255, 255, 255, 255);
	}
	else {
		mConnectButton.setFillColor(sf::Color(0, 0, 0, 150));

		sf::VertexArray& backVertices = mBackButton.getVertices();
		backVertices[0].color =
		backVertices[1].color =
		backVertices[2].color =
		backVertices[3].color = sf::Color(255, 255, 255, 80);
	}

	mIpTextbox.update(dt);
	mPortTextbox.update(dt);

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

const std::string MultiplayerConnectState::getHostIP()
{
	return static_cast<const std::string>(mIpTextbox.getText()->getString());
}

const std::string MultiplayerConnectState::getHostPort()
{
	return static_cast<const std::string>(mPortTextbox.getText()->getString());
}