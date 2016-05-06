#include "BasePlayer.h"

#include <SFML/Graphics/RenderTarget.hpp>

BasePlayer::BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	                   const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	                   std::vector<gStruct::UnitData>& unitData,
	                   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
	                   std::vector<gStruct::TurretData>& turretData,
	                   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(Side::Ally, worldBounds, baseTexture, unitTextures, unitData, turretTextures, turretData, soundPlayer)
	, mUnitButtons(window, unitTextures, unitData)
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
	mUnitButtons.updateButtonOverlay(mGold);
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
		handleUnitSpawn(static_cast<Unit::UnitType>(i));

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1)
	{
		mTurrets.emplace_back(Turret(mSide, sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height),
		                             mTurretData[Turret::LaserTurret], mTurretTextures));

		sf::FloatRect turretGBounds(mTurrets.back().getGlobalBounds());
		mTurrets.back().setPosition(getPosition().x + getGlobalBounds().width / 2.f - turretGBounds.width / 3.f,
			                        getPosition().y + turretGBounds.height / 2.f);
	}
}

void BasePlayer::update(sf::Time dt)
{
	Base::update(dt);
	mUnitButtons.update();
}

void BasePlayer::modifyGold(int amount)
{
	mGold += amount;
	updateGoldGUI();
}