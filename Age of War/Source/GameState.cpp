#include "GameState.h"

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mUnitData(std::move(gStruct::initializeUnitData()))
	, mBasePlayer(nullptr)
	, mBaseOpponent(nullptr)
	, mBackground(sf::Quads, 4)
	, mPlaying(true)
{
	setupResources();
	setupBackground();

	sf::Vector2u winSize(mWindow.getSize());
	mBasePlayer = std::unique_ptr<BasePlayer>(new BasePlayer(mWindow, sf::IntRect(0, 0, winSize.x, winSize.y),
											                 mBaseTexture, mUnitTextures, mUnitData, mSoundPlayer));
	mBaseOpponent = std::unique_ptr<Base>(new BaseAI(Entity::Side::Right, sf::IntRect(0, 0, winSize.x, winSize.y),
													 mBaseTexture, mUnitTextures, mUnitData, mSoundPlayer));

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

	mBackground[0].position = sf::Vector2f(0.f, 0.f);
	mBackground[1].position = sf::Vector2f(winSize.x, 0.f);
	mBackground[2].position = winSize;
	mBackground[3].position = sf::Vector2f(0.f, winSize.y);

	mBackground[0].texCoords = sf::Vector2f(0.f, 0.f);
	mBackground[1].texCoords = sf::Vector2f(textureSize.x, 0.f);
	mBackground[2].texCoords = textureSize;
	mBackground[3].texCoords = sf::Vector2f(0.f, textureSize.y);
}

void GameState::setupResources()
{
	mUnitTextures.load(Unit::Type::Mage, "Assets/Textures/Mage.png");
	mUnitTextures.load(Unit::Type::Knight, "Assets/Textures/Knight.png");
	mUnitTextures.load(Unit::Type::Destroyer, "Assets/Textures/Destroyer.png");

	mTurretTextures.load(Turret::Type::Turret1, "Assets/Textures/Turret1.png");

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
	{
		mPlaying = false;
		requestStateClear();
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStatePush(pyro::StateID::Pause);
		mMusicPlayer.pause(true);
	}

	mBasePlayer->handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	if (mBasePlayer->hasUnits() && mBaseOpponent->hasUnits())
	{
		mBasePlayer->attack(mBaseOpponent->getFirstUnit());
		mBaseOpponent->attack(mBasePlayer->getFirstUnit());
	}
	else {
		mBasePlayer->attack(mBaseOpponent.get());
		mBaseOpponent->attack(mBasePlayer.get());
	}

	mBasePlayer->update(dt);
	mBaseOpponent->update(dt);

	return true;
}

void GameState::draw()
{
	mWindow.draw(mBackground, &mBackgroundTexture);
	mWindow.draw(*mBasePlayer);
	mWindow.draw(*mBaseOpponent);
}