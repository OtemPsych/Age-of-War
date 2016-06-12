#include "GameState.h"

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mUnitData(std::move(gStruct::initializeUnitData()))
	, mTurretData(std::move(gStruct::initializeTurretData()))
	, mBasePlayer(nullptr)
	, mBaseOpponent(nullptr)
{
	setupResources();
	setupBackground();

	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursorTexture.setSmooth(true);
	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);

	sf::Vector2u winSize(mWindow.getSize());
	mBasePlayer = std::unique_ptr<BasePlayer>(new BasePlayer(mWindow, mWorldBounds, mBaseTexture,
		                                                     mUnitTextures, mUnitData, mTurretTextures, mTurretData, mSoundPlayer));
	mBaseOpponent = std::unique_ptr<Base>(new BaseAI(Entity::Side::Enemy, mWorldBounds, mBaseTexture,
		                                             mUnitTextures, mUnitData, mTurretTextures, mTurretData, mSoundPlayer));

	mMusicPlayer.setVolume(50.f);
	mMusicPlayer.play(MusicID::Soundtrack);
}

GameState::~GameState()
{
}

void GameState::setupBackground()
{
	sf::Vector2f winSize(mWindow.getSize());
	sf::Vector2f textureSize(mBackgroundTexture.getSize());

	mWorldBounds = sf::IntRect(0, 0, static_cast<int>(textureSize.x) * 2, static_cast<int>(textureSize.y));

	mBackgroundTexture.setRepeated(true);
	mBackground.setTexture(mBackgroundTexture);
	mBackground.setTextureRect(mWorldBounds);
}

void GameState::setupResources()
{
	mUnitTextures.load(Unit::UnitType::Mage, "Assets/Textures/Mage.png");
	mUnitTextures.load(Unit::UnitType::Knight, "Assets/Textures/Knight.png");
	mUnitTextures.load(Unit::UnitType::Destroyer, "Assets/Textures/Destroyer.png");
	mUnitTextures.load(Unit::UnitType::Executioner, "Assets/Textures/Executioner.png");
	mUnitTextures.load(Unit::UnitType::Shadow, "Assets/Textures/Shadow.png");
	mUnitTextures.load(Unit::UnitType::Samurai, "Assets/Textures/Samurai.png");

	mTurretTextures.load(Turret::LaserTurret, "Assets/Textures/LaserTurret.png");

	mBackgroundTexture.loadFromFile("Assets/Textures/Background.png");
	mBaseTexture.loadFromFile("Assets/Textures/Base.png");

	mMusicPlayer.loadTheme(MusicID::Soundtrack, "Assets/Music/Soundtrack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::MageAttack, "Assets/Sounds/MageAttack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::KnightAttack, "Assets/Sounds/KnightAttack.ogg");
}

void GameState::unpauseMusic()
{
	mMusicPlayer.pause(false);
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
		requestStateClear();
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		requestStatePush(pyro::StateID::Pause);
		mMusicPlayer.pause(true);
	}

	mBasePlayer->handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	mBasePlayer->update(dt);
	mBaseOpponent->update(dt);

	if (mBasePlayer->hasUnits())
		mBaseOpponent->attack(mBasePlayer->getFirstUnit());
	else
		mBaseOpponent->attack(*mBasePlayer);

	if (mBaseOpponent->hasUnits())
		mBasePlayer->attack(mBaseOpponent->getFirstUnit());
	else
		mBasePlayer->attack(*mBaseOpponent);

	sf::View newView(mWindow.getView());
	const float coordsX = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)).x;
	const float halfViewWidth = newView.getSize().x / 2.f;
	const sf::Vector2f viewCenter = newView.getCenter();
	const float movement = 7.f;

	if (coordsX >= viewCenter.x + halfViewWidth - 100.f) {
		const float totalDisplacement = viewCenter.x + halfViewWidth + movement;
		sf::View newView(mWindow.getView());
		if (totalDisplacement < mWorldBounds.width)
			newView.move(movement, 0.f);
		else
			newView.setCenter(mWorldBounds.width - halfViewWidth, viewCenter.y);

		mWindow.setView(newView);
	}
	else if (coordsX <= viewCenter.x - halfViewWidth + 100.f) {
		const float totalDisplacement = viewCenter.x - halfViewWidth - movement;
		sf::View newView(mWindow.getView());
		if (totalDisplacement > mWorldBounds.left)
			newView.move(-movement, 0.f);
		else
			newView.setCenter(mWorldBounds.left + halfViewWidth, viewCenter.y);

		mWindow.setView(newView);
	}

	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));

	return true;
}

void GameState::draw()
{
	mWindow.draw(mBackground, &mBackgroundTexture);
	mWindow.draw(*mBasePlayer);
	mWindow.draw(*mBaseOpponent);

	mWindow.draw(mCursor);
}