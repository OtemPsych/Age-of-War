#include "GameState.h"
#include "GameOverState.h"

#include <PYRO/StateStack.h>

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mUnitData(std::move(gStruct::initializeUnitData()))
	, mTurretData(std::move(gStruct::initializeTurretData()))
	, mBasePlayer(nullptr)
	, mBaseOpponent(nullptr)
	, mWindowBounds(0, 0, mWindow.getSize().x, mWindow.getSize().y)
{
	setupResources();
	setupBackground();

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

	mBackgroundTexture.setRepeated(true);
	mBackground.setTexture(mBackgroundTexture);
	mBackground.scale(1.f, mWindow.getSize().y / textureSize.y);
	mBackground.setTextureRect(sf::IntRect(0, 0, static_cast<int>(textureSize.x * 1.5f), static_cast<int>(textureSize.y)));

	mWorldBounds = mBackground.getTextureRect();
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

	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursorTexture.setSmooth(true);
	mBackgroundTexture.loadFromFile("Assets/Textures/Background.png");
	mBaseTexture.loadFromFile("Assets/Textures/Base.png");

	mMusicPlayer.loadTheme(MusicID::Soundtrack, "Assets/Music/Soundtrack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::MageAttack, "Assets/Sounds/MageAttack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::KnightAttack, "Assets/Sounds/KnightAttack.ogg");
}

void GameState::unpauseMusic()
{
	mMusicPlayer.pause(false);
	mCursor.setPosition(mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));
}

bool GameState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		|| event.type == sf::Event::LostFocus) {
		requestStatePush(pyro::StateID::Pause);
		mMusicPlayer.pause(true);
	}
	else if (event.type == sf::Event::Closed) {
		requestStateClear();
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
	else {
		mBaseOpponent->attack(*mBasePlayer);
		if (mBasePlayer->isDestroyable()) {
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(mStack.getState(pyro::StateID::Other1)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Defeat);
			} else {
				requestStatePush(pyro::StateID::Other1);
				mMusicPlayer.stop();
			}
		}
	}

	if (mBaseOpponent->hasUnits())
		mBasePlayer->attack(mBaseOpponent->getFirstUnit());
	else {
		mBasePlayer->attack(*mBaseOpponent);
		if (mBaseOpponent->isDestroyable()) {
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(mStack.getState(pyro::StateID::Other1)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Victory);
			} else {
				requestStatePush(pyro::StateID::Other1);
				mMusicPlayer.stop();
			}
		}
	}

	if (mWindowBounds.contains(sf::Mouse::getPosition(mWindow)))
	{
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
			mBasePlayer->updateGUIPositions();
		}
		else if (coordsX <= viewCenter.x - halfViewWidth + 100.f) {
			const float totalDisplacement = viewCenter.x - halfViewWidth - movement;
			sf::View newView(mWindow.getView());
			if (totalDisplacement > mWorldBounds.left)
				newView.move(-movement, 0.f);
			else
				newView.setCenter(mWorldBounds.left + halfViewWidth, viewCenter.y);

			mWindow.setView(newView);
			mBasePlayer->updateGUIPositions();
		}
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