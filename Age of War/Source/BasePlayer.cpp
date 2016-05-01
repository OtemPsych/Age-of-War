#include "BasePlayer.h"

#include <SFML/Graphics/RenderTarget.hpp>

BasePlayer::BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds,
					   const sf::Texture& baseTexture, const pyro::TextureHolder<Unit::Type>& textures,
	                   std::vector<gStruct::UnitData>& data,
	                   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(Side::Left, worldBounds, baseTexture, textures, data, soundPlayer)
	, mUnitButtons(window, textures, data)
{
	setupGoldGUI();
}

void BasePlayer::setupGoldGUI()
{
	mGoldCoinTexture.loadFromFile("Assets/Textures/GoldCoin.png");
	mGoldCoinSprite.setTexture(mGoldCoinTexture);
	mGoldCoinSprite.setPosition(10.f, 10.f);

	mFont.loadFromFile("Assets/Fonts/Gold.ttf");
	mGoldText.setFont(mFont);
	mGoldText.setCharacterSize(45);
	mGoldText.setColor(sf::Color::Black);
	updateGoldGUI();
}

void BasePlayer::updateGoldGUI()
{
	mUnitButtons.update(mGold);
	mGoldText.setString(std::to_string(mGold));
	mGoldText.setPosition(mGoldCoinSprite.getGlobalBounds().width + mGoldText.getGlobalBounds().width / 2.f + 15.f,
		                  mGoldCoinSprite.getGlobalBounds().height / 2.f);
}

void BasePlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Base::draw(target, states);

	target.draw(mUnitButtons, states);

	target.draw(mGoldCoinSprite, states);
	target.draw(mGoldText, states.transform *= mGoldCoinSprite.getTransform());
}

void BasePlayer::handleEvent(const sf::Event& event)
{
	int i = mUnitButtons.handleEvent(event);
	if (i != -1)
		handleUnitSpawn(static_cast<Unit::Type>(i));
}

void BasePlayer::modifyGold(int amount)
{
	mGold += amount;
	updateGoldGUI();
}