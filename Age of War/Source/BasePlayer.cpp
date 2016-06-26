#include "BasePlayer.h"
#include "Tools/TextureDataReader.h"

BasePlayer::BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds, sf::Font& font, const sf::Texture& baseTexture,
	                   const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	                   std::vector<gStruct::UnitData>& unitData,
	                   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
	                   std::vector<gStruct::TurretData>& turretData,
	                   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(Side::Ally, worldBounds, font, baseTexture, unitTextures, unitData, turretTextures, turretData, soundPlayer)
	, mCoinRotateAnimation(mCoinSprite, readTextureData("Coin", "Rotate"), sf::seconds(0.8f), true)
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
	mCoinTexture.loadFromFile("Assets/Textures/Coin.png");
	mCoinSprite.setTexture(mCoinTexture);
	mCoinSprite.setPosition(30.f, 30.f);

	mCoinFont.loadFromFile("Assets/Fonts/Gold.ttf");
	mCoinText.setFont(mCoinFont);
	mCoinText.setCharacterSize(45);
	mCoinText.setTextColor(sf::Color::Black);
	mCoinText.activateShadow(true);
	mCoinText.setShadowOffset(3.f, 2.f);
	mCoinText.setShadowColor(sf::Color(255, 255, 255, 150));
	mCoinText.setOriginFlags(pyro::utils::OriginFlags::Left | pyro::utils::OriginFlags::CenterY);
	mCoinText.setPosition(60.f, mCoinSprite.getPosition().y);
	updateGoldGUI();
}

void BasePlayer::updateGoldGUI()
{
	mUnitButtons.updateButtonOverlay(mGold);
	mTurretButtons.updateButtonOverlay(mGold);

	mCoinText.setString(std::to_string(mGold));
}

void BasePlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Base::draw(target, states);

	if (mActiveTurretPlacementIndicators) {
		target.draw(mTurretPlacementIndicators, states.transform * getTransform());
		target.draw(*mTurretIndicator, states);
	}

	target.draw(mUnitButtons, states);
	target.draw(mTurretButtons, states);

	target.draw(mCoinSprite, states);
	target.draw(mCoinText, states);
}

void BasePlayer::updateGUIPositions()
{
	const float viewCenterX = mWindow.getView().getCenter().x;
	const float halfViewX = mWindow.getView().getSize().x / 2.f;

	mUnitButtons.setPosition(viewCenterX - halfViewX, mUnitButtons.getPosition().y);
	mTurretButtons.setPosition(viewCenterX - halfViewX, mTurretButtons.getPosition().y);
	mCoinSprite.setPosition(viewCenterX - halfViewX + 30.f, mCoinSprite.getPosition().y);
	mCoinText.setPosition(viewCenterX - halfViewX + 60.f, mCoinSprite.getPosition().y);
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

		mTurretIndicator = std::make_unique<sf::RectangleShape>(mTurretButtons.getButtonBox(j));
		sf::FloatRect lBounds(mTurretIndicator->getLocalBounds());
		mTurretIndicator->setOrigin(lBounds.width / 3.f, lBounds.height / 3.f);
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

	mCoinRotateAnimation.update(dt);
}

void BasePlayer::modifyGold(int amount)
{
	mGold += amount;
	updateGoldGUI();
}