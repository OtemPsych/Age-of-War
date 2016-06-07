#include "BasePlayer.h"

BasePlayer::BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	                   const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	                   std::vector<gStruct::UnitData>& unitData,
	                   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
	                   std::vector<gStruct::TurretData>& turretData,
	                   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(Side::Ally, worldBounds, baseTexture, unitTextures, unitData, turretTextures, turretData, soundPlayer)
	, mUnitButtons(unitData, window, unitTextures, sf::Vector2f(55.f, 60.f))
	, mTurretButtons(turretData, window, turretTextures, sf::Vector2f(55.f, 25.f))
	, mActiveTurretPlacementIndicators(false)
	, mTurretIndicator(nullptr)
	, mTurretPlacementIndicators(mTurretWindowRects, window)
	, mWindow(window)
{
	setupGoldGUI();

	mTurretPlacementIndicators.setPosition(getPosition());
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
	mTurretButtons.updateButtonOverlay(mGold);

	mGoldText.setString(std::to_string(mGold));
	mGoldText.setPosition(mGoldCoinSprite.getGlobalBounds().width + mGoldText.getGlobalBounds().width / 2.f + 15.f,
		                  mGoldCoinSprite.getGlobalBounds().height / 2.f);
}

void BasePlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Base::draw(target, states);

	target.draw(mUnitButtons, states);
	target.draw(mTurretButtons, states);

	if (mActiveTurretPlacementIndicators) {
		target.draw(mTurretPlacementIndicators, states.transform * getTransform());
		target.draw(*mTurretIndicator, states);
	}

	target.draw(mGoldCoinSprite, states);
	target.draw(mGoldText, states.transform *= mGoldCoinSprite.getTransform());
}

void BasePlayer::handleEvent(const sf::Event& event)
{
	int i = mUnitButtons.handleEvent(event);
	if (i != -1)
		handleUnitSpawn(static_cast<Unit::UnitType>(i));

	int j = mTurretButtons.handleEvent(event);
	if (j != -1 && mGold >= mTurretData[j].cost) {
		mActiveTurretPlacementIndicators = true;
		mTurretTypeToSpawn = j;

		mTurretIndicator = std::make_unique<sf::RectangleShape>(mTurretButtons.getButtonShape(j));
		mTurretIndicator->setOutlineThickness(0.f);
	}

	if (mActiveTurretPlacementIndicators)
	{
		if (mTurretPlacementIndicators.handleEvent(event)) {
			handleTurretSpawn(static_cast<Turret::TurretType>(mTurretTypeToSpawn), mTurretPlacementIndicators.getTurretIndicator());
			mActiveTurretPlacementIndicators = false;
			mTurretTypeToSpawn = -1;
			mTurretIndicator.reset();
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			mActiveTurretPlacementIndicators = false;
			mTurretIndicator.reset();
		}
	}
}

void BasePlayer::update(sf::Time dt)
{
	Base::update(dt);

	mUnitButtons.update();
	mTurretButtons.update();

	if (mActiveTurretPlacementIndicators) {
		mTurretPlacementIndicators.update();
		mTurretIndicator->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));
	}
}

void BasePlayer::modifyGold(int amount)
{
	mGold += amount;
	updateGoldGUI();
}