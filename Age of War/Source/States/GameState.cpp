#include "GameState.h"
#include "GameOverState.h"

#include <PYRO/StateStack.h>

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, mBasePlayer(nullptr)
	, mBaseOpponent(nullptr)
	, mWindowBounds(0, 0, window->getSize().x, window->getSize().y)
{
	setupResources();
	setupBackground();

	mCursor.setTexture(mCursorTexture);
	mCursor.scale(0.9f, 0.9f);

	sf::Vector2u winSize(window->getSize());
	mBasePlayer = std::unique_ptr<BasePlayer>(new BasePlayer(*window, mWorldBounds, mDisplayDamageFont, mBaseTexture,
		                                                     mUnitTextures, mTurretTextures, mSoundPlayer));
	mBaseOpponent = std::unique_ptr<Base>(new BaseAI(Entity::Side::Enemy, mWorldBounds, mDisplayDamageFont, mBaseTexture,
		                                             mUnitTextures, mTurretTextures, mSoundPlayer));

	mMusicPlayer.setVolume(75.f);
	mMusicPlayer.play(MusicID::Soundtrack, true);
}

GameState::~GameState()
{
}

void GameState::setupBackground()
{
	sf::Vector2f winSize(window_->getSize());
	sf::Vector2f textureSize(mBackgroundTexture.getSize());

	mBackgroundTexture.setRepeated(true);
	mBackground.setTexture(mBackgroundTexture);
	mBackground.scale(1.f, window_->getSize().y / textureSize.y);
	mBackground.setTextureRect(sf::IntRect(0, 0, static_cast<int>(textureSize.x * 1.5f), static_cast<int>(textureSize.y)));

	mWorldBounds = mBackground.getTextureRect();
}

void GameState::setupResources()
{
	mUnitTextures.load("Assets/Textures/Mage.png", Unit::UnitType::Mage);
	mUnitTextures.load("Assets/Textures/Knight.png", Unit::UnitType::Knight);
	mUnitTextures.load("Assets/Textures/Destroyer.png", Unit::UnitType::Destroyer);
	mUnitTextures.load("Assets/Textures/Executioner.png", Unit::UnitType::Executioner);
	mUnitTextures.load("Assets/Textures/Shadow.png", Unit::UnitType::Shadow);
	mUnitTextures.load("Assets/Textures/Samurai.png", Unit::UnitType::Samurai);

	mTurretTextures.load("Assets/Textures/LaserTurret.png", Turret::LaserTurret);

	mCursorTexture.loadFromFile("Assets/Textures/MouseCursor.png");
	mCursorTexture.setSmooth(true);
	mBackgroundTexture.loadFromFile("Assets/Textures/Background.png");
	mBaseTexture.loadFromFile("Assets/Textures/Base.png");

	mDisplayDamageFont.loadFromFile("Assets/Fonts/Zombie.ttf");

	mMusicPlayer.loadTheme(MusicID::Soundtrack, "Assets/Music/Soundtrack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::MageAttack, "Assets/Sounds/MageAttack.ogg");
	mSoundPlayer.loadEffect(Unit::SoundID::KnightAttack, "Assets/Sounds/KnightAttack.ogg");
}

void GameState::unpauseMusic()
{
	mMusicPlayer.pause(false);
	mCursor.setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
}

void GameState::updateCamera()
{
	if (mWindowBounds.contains(sf::Mouse::getPosition(*window_)))
	{
		sf::View newView(window_->getView());
		const sf::Vector2f viewCenter = newView.getCenter();
		const sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
		const float coordsX = window_->mapPixelToCoords(mousePos).x;
		const float halfViewWidth = newView.getSize().x / 2.f;
		const float movement = 7.f;
		const float scrollEdge = 100.f;

		if (coordsX >= viewCenter.x + halfViewWidth - scrollEdge) {
			const float totalDisplacement = viewCenter.x + halfViewWidth + movement;
			sf::View newView(window_->getView());
			if (totalDisplacement < mWorldBounds.width)
				newView.move(-(mWindowBounds.width - mousePos.x - scrollEdge) * movement / 100.f, 0.f);
			else
				newView.setCenter(mWorldBounds.width - halfViewWidth, viewCenter.y);

			window_->setView(newView);
			mBasePlayer->updateGUIPositions();
		}
		else if (coordsX <= viewCenter.x - halfViewWidth + scrollEdge) {
			const float totalDisplacement = viewCenter.x - halfViewWidth - movement;
			sf::View newView(window_->getView());
			if (totalDisplacement > mWorldBounds.left)
				newView.move(-(scrollEdge - mousePos.x) * movement / 100.f, 0.f);
			else
				newView.setCenter(mWorldBounds.left + halfViewWidth, viewCenter.y);

			window_->setView(newView);
			mBasePlayer->updateGUIPositions();
		}
	}
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
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(stack_->getState(pyro::StateID::GameOver)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Defeat);
			} else {
				requestStatePush(pyro::StateID::GameOver);
				mMusicPlayer.stop();
			}
		}
	}

	if (mBaseOpponent->hasUnits())
		mBasePlayer->attack(mBaseOpponent->getFirstUnit());
	else {
		mBasePlayer->attack(*mBaseOpponent);
		if (mBaseOpponent->isDestroyable()) {
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(stack_->getState(pyro::StateID::GameOver)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Victory);
			} else {
				requestStatePush(pyro::StateID::GameOver);
				mMusicPlayer.stop();
			}
		}
	}

	updateCamera();
	mCursor.setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));

	return true;
}

void GameState::draw()
{
	window_->draw(mBackground, &mBackgroundTexture);
	window_->draw(*mBasePlayer);
	window_->draw(*mBaseOpponent);

	mBasePlayer->drawUnitDamageDisplays(*window_, sf::RenderStates::Default);
	mBaseOpponent->drawUnitDamageDisplays(*window_, sf::RenderStates::Default);

	window_->draw(mCursor);
}