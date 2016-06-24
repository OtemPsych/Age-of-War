#include "GameOverState.h"

#include <PYRO/Utils.h>

#include <SFML/Graphics/Shader.hpp>

GameOverState::GameOverState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mGameOverType(GameOverType::None)
	, mBackgroundTint(sf::Quads)
	, mContinueButton(window, sf::Vector2f(120.f, 40.f))
{
	setupResources();

	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);
	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));

	const sf::Vector2f viewSize(window.getSize());
	const sf::Vector2f viewCenter(window.getView().getCenter());
	const sf::FloatRect view(viewCenter - viewSize / 2.f, viewSize);

	mBackgroundTint.append(sf::Vertex(sf::Vector2f(view.left, view.top)));
	mBackgroundTint.append(sf::Vertex(sf::Vector2f(view.left + view.width, view.top)));
	mBackgroundTint.append(sf::Vertex(sf::Vector2f(view.left + view.width, view.top + view.height)));
	mBackgroundTint.append(sf::Vertex(sf::Vector2f(view.left, view.top + view.height)));

	mContinueButton.activateText(true);
	pyro::Text* continueText = mContinueButton.getText();
	continueText->setFont(mFont);
	continueText->setOriginFlags(pyro::utils::OriginFlags::Center);
	continueText->setString("Continue");
	continueText->setPosition(mContinueButton.getSize() / 2.f);
	continueText->setShadowColor(sf::Color::Transparent);
}

void GameOverState::setupResources()
{
	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");

	mFont.loadFromFile("Assets/Fonts/Menu.ttf");
}

void GameOverState::setGameOverType(GameOverState::GameOverType type)
{
	mGameOverType = type;

	if (type == GameOverType::Victory) {
		mBackgroundTint[0].color =
		mBackgroundTint[1].color =
		mBackgroundTint[2].color =
		mBackgroundTint[3].color = sf::Color(51, 153, 255, 50);
		mContinueButton.setFillColor(sf::Color(51, 153, 255, 200));

		mSpriteTexture.loadFromFile("Assets/Textures/DisplayVictory.png");
	} else {
		mBackgroundTint[0].color =
		mBackgroundTint[1].color =
		mBackgroundTint[2].color =
		mBackgroundTint[3].color = sf::Color(153, 0, 0, 50);
		mContinueButton.setFillColor(sf::Color(153, 0, 0, 225));

		mSpriteTexture.loadFromFile("Assets/Textures/DisplayDefeat.png");
	}

	mSpriteTexture.setSmooth(true);
	mSprite.setTexture(mSpriteTexture);
	sf::FloatRect lBounds(mSprite.getLocalBounds());
	mSprite.setOrigin(lBounds.width / 2.f, 0.f);
	mSprite.setPosition(mWindow.getView().getCenter().x, 0.f);
	mContinueButton.setPosition(mWindow.getView().getCenter().x, mSprite.getGlobalBounds().height - 50.f);
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));
	} 
	else if (mContinueButton.clicked(event)) {
		mWindow.setView(mWindow.getDefaultView());

		requestStateClear();
		requestStatePush(pyro::StateID::Menu);
	}
	else if (event.type == sf::Event::Closed) {
		requestStateClear();
	}

	return mGameOverType == GameOverType::None;
}

bool GameOverState::update(sf::Time dt)
{
	sf::Color standardColor, hoverColor;
	if (mGameOverType == GameOverType::Victory) {
		standardColor = sf::Color(51, 153, 255, 200);
		hoverColor = sf::Color(128, 191, 255, 200);
	}
	else if (mGameOverType == GameOverType::Defeat) {
		standardColor = sf::Color(153, 0, 0, 225);
		hoverColor = sf::Color(204, 0, 0, 225);
	}
	mContinueButton.setFillColor(mContinueButton.hover() ? hoverColor : standardColor);

	return mGameOverType == GameOverType::None;
}

void GameOverState::draw()
{
	if (mGameOverType != GameOverType::None) {
		mWindow.draw(mBackgroundTint);
		mWindow.draw(mSprite);
		mWindow.draw(mContinueButton);
		mWindow.draw(mCursor);
	}
}