#include "PauseState.h"
#include "GameState.h"
#include <PYRO/StateStack.h>

PauseState::PauseState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mBackgroundBlur(sf::Quads, 4)
	, mBackground(sf::Quads, 4)
{
	mFont.loadFromFile("Assets/Fonts/Menu.ttf");

	const sf::Vector2f winSize(window.getSize());
	const sf::Vector2f buttonSize(350.f, 50.f);
	const float margin = 15.f;
	for (unsigned i = 0; i < TypeCount; i++)
	{
		mButtons.emplace_back(pyro::gui::ClickableGUIEntity(window, buttonSize));
		mButtons.back().setPosition(winSize.x / 2.f, winSize.y / 2.f + (buttonSize.y + margin) * i);
		mButtons.back().getText().setFont(mFont);
		mButtons.back().getText().setColor(sf::Color(255, 255, 255, 200));
		mButtons.back().getShape().setFillColor(sf::Color(0, 0, 0, 0));
	}

	mButtons[Resume].getText().setString("Resume");
	mButtons[Restart].getText().setString("Restart");
	mButtons[Quit_Menu].getText().setString("Quit to Main Menu");
	mButtons[Quit_Desktop].getText().setString("Quit to Desktop");

	mBackgroundBlur[0].position = sf::Vector2f(0.f, 0.f);
	mBackgroundBlur[1].position = sf::Vector2f(winSize.x, 0.f);
	mBackgroundBlur[2].position = winSize;
	mBackgroundBlur[3].position = sf::Vector2f(0.f, winSize.y);

	for (unsigned i = 0; i < 4; i++)
		mBackgroundBlur[i].color = sf::Color(0, 0, 0, 150);

	const float topPosY = mButtons.front().getPosition().y - buttonSize.y / 2.f - margin;
	const float botPosY = mButtons.back().getPosition().y + buttonSize.y / 2.f + margin;

	mBackground[0].position = sf::Vector2f(0.f, topPosY);
	mBackground[1].position = sf::Vector2f(winSize.x, topPosY);
	mBackground[2].position = sf::Vector2f(winSize.x, botPosY);
	mBackground[3].position = sf::Vector2f(0.f, botPosY);

	for (unsigned i = 0; i < 4; i++)
		mBackground[i].color = sf::Color(0, 0, 0, 200);
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (mButtons[Resume].clicked(event) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
	{
		requestStatePop();
		const_cast<GameState*>(dynamic_cast<const GameState*>(mStack.getState(pyro::StateID::Game)))->unpauseMusic();
	}
	else if (mButtons[Restart].clicked(event))
	{
		requestStateClear();
		requestStatePush(pyro::StateID::Game);
	}
	else if (mButtons[Quit_Menu].clicked(event))
	{
		requestStateClear();
		requestStatePush(pyro::StateID::Menu);
	}
	else if (event.type == sf::Event::Closed || mButtons[Quit_Desktop].clicked(event))
		requestStateClear();

	return false;
}

bool PauseState::update(sf::Time dt)
{
	for (auto& button : mButtons)
		if (button.hover())
			button.getText().setColor(sf::Color(255, 255, 255, 255));
		else
			button.getText().setColor(sf::Color(255, 255, 255, 200));

	return false;
}

void PauseState::draw()
{
	mWindow.draw(mBackgroundBlur);
	mWindow.draw(mBackground);
	for (const auto& button : mButtons)
		mWindow.draw(button);
}