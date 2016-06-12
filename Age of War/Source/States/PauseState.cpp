#include "PauseState.h"
#include "GameState.h"

#include <PYRO/StateStack.h>
#include <PYRO/Utils.h>

PauseState::PauseState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mBackgroundBlur(sf::Quads, 4)
	, mBackground(sf::Quads, 4)
{
	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");

	const sf::Vector2f viewSize(window.getSize());
	const sf::Vector2f viewCenter(window.getView().getCenter());
	const sf::FloatRect view(viewCenter - viewSize / 2.f, viewSize);
	const sf::Vector2f buttonSize(350.f, 50.f);
	const float margin = 15.f;
	for (unsigned i = 0; i < TypeCount; i++)
	{
		mButtons.emplace_back(pyro::gui::ClickableGUIEntity(window, buttonSize));
		sf::RectangleShape& box(mButtons.back().getBox());
		pyro::Text& text(mButtons.back().getText());

		text.setFont(mFont);
		text.setShadowOffset(2.f, 2.f);
		text.setShadowColor(sf::Color(255, 255, 255, 45));
		text.setOriginFlags(pyro::utils::OriginFlags::Center);
		text.setTextColor(sf::Color(255, 255, 255, 150));
		text.setPosition(box.getSize() / 2.f);
		box.setFillColor(sf::Color::Transparent);
		mButtons.back().setPosition(viewCenter.x, viewCenter.y + (buttonSize.y + margin) * i);
		mButtons.back().setOriginFlags(pyro::utils::OriginFlags::Center);
	}

	mButtons[Resume].getText().setString("Resume");
	mButtons[Restart].getText().setString("Restart");
	mButtons[Quit_Menu].getText().setString("Quit to Main Menu");
	mButtons[Quit_Desktop].getText().setString("Quit to Desktop");

	mBackgroundBlur[0].position = sf::Vector2f(view.left, view.top);
	mBackgroundBlur[1].position = sf::Vector2f(view.left + view.width, view.top);
	mBackgroundBlur[2].position = sf::Vector2f(view.left + view.width, view.top + view.height);
	mBackgroundBlur[3].position = sf::Vector2f(view.left, view.top + view.height);

	for (unsigned i = 0; i < 4; i++)
		mBackgroundBlur[i].color = sf::Color(0, 0, 0, 150);

	const float topPosY = mButtons.front().getPosition().y - buttonSize.y / 2.f - margin;
	const float botPosY = mButtons.back().getPosition().y + buttonSize.y / 2.f + margin;

	mBackground[0].position = sf::Vector2f(view.left, topPosY);
	mBackground[1].position = sf::Vector2f(view.left + view.width, topPosY);
	mBackground[2].position = sf::Vector2f(view.left + view.width, botPosY);
	mBackground[3].position = sf::Vector2f(view.left, botPosY);

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
		mWindow.setView(mWindow.getDefaultView());

		requestStateClear();
		requestStatePush(pyro::StateID::Game);
	}
	else if (mButtons[Quit_Menu].clicked(event))
	{
		mWindow.setView(mWindow.getDefaultView());

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
		if (button.hover()) {
			button.getText().setTextColor(sf::Color(255, 255, 255, 255));
			break;
		}
		else
			button.getText().setTextColor(sf::Color(255, 255, 255, 150));

	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));

	return false;
}

void PauseState::draw()
{
	mWindow.draw(mBackgroundBlur);
	mWindow.draw(mBackground);
	for (const auto& button : mButtons)
		mWindow.draw(button);

	mWindow.draw(mCursor);
}