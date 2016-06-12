#include "MenuState.h"

#include <PYRO/StateStack.h>
#include <PYRO/Utils.h>

MenuState::MenuState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
{
	window.setMouseCursorVisible(false);

	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);

	mBackgroundTexture.loadFromFile("Assets/Textures/MenuBackground.jpg");
	mBackgroundSprite.setTexture(mBackgroundTexture);

	sf::Vector2f winSize(mWindow.getSize());
	sf::Vector2f textureSize(mBackgroundTexture.getSize());
	mBackgroundSprite.scale(winSize.x / textureSize.x, winSize.y / textureSize.y);

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");
	const sf::Vector2f buttonSize(150.f, 50.f);
	const float margin = 15.f;
	for (unsigned i = 0; i < TypeCount; i++)
	{
		mButtons.emplace_back(pyro::gui::ClickableGUIEntity(window, buttonSize));
		sf::RectangleShape& box(mButtons.back().getBox());
		pyro::Text& text(mButtons.back().getText());

		box.setFillColor(sf::Color(0, 0, 0, 150));
		text.setFont(mFont);
		text.setTextColor(sf::Color::White);
		text.setShadowOffset(2.f, 2.f);
		text.setShadowColor(sf::Color(255, 255, 255, 50));
		text.setOriginFlags(pyro::utils::OriginFlags::Center);
		text.setPosition(box.getSize() / 2.f);
		mButtons.back().setOriginFlags(pyro::utils::OriginFlags::Center);
		mButtons.back().setPosition(winSize.x / 2.f, winSize.y / 2.f + (buttonSize.y + margin) * i);
	}

	mButtons[Play].getText().setString("Play");
	mButtons[Multiplayer].getText().setString("Multiplayer");
	mButtons[Quit].getText().setString("Quit");
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (mButtons[Play].clicked(event)) 
	{
		requestStatePop();
		requestStatePush(pyro::StateID::Game);
	} 
	else if (mButtons[Multiplayer].clicked(event)) 
	{
		requestStatePop();
		requestStatePush(pyro::StateID::MultiplayerConnect);
	} 
	else if (event.type == sf::Event::Closed || mButtons[Quit].clicked(event))
		requestStateClear();

	return true;
}

bool MenuState::update(sf::Time dt)
{
	for (auto& button : mButtons)
		if (button.hover()) {
			button.getBox().setFillColor(sf::Color(255, 255, 255, 120));
			break;
		}
		else
			button.getBox().setFillColor(sf::Color(0, 0, 0, 150));

	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));

	return true;
}

void MenuState::draw()
{
	mWindow.draw(mBackgroundSprite);

	for (const auto& button : mButtons)
		mWindow.draw(button);

	mWindow.draw(mCursor);
}