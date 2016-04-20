#include "GameState.h"

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mUnitData(std::move(gStruct::initializeUnitData()))
	, mLeftBase(nullptr)
	, mRightBase(nullptr)
	, mBackground(sf::Quads, 4)
{
	mWindow.setFramerateLimit(144);

	setupResources();
	setupBackground();

	sf::Vector2u winSize(mWindow.getSize());
	mLeftBase = std::unique_ptr<Base>(new Base(Entity::Side::Left, sf::IntRect(0, 0, winSize.x, winSize.y),
											   mBaseTexture, mUnitTextures, mUnitData, mSoundPlayer));
	mRightBase = std::unique_ptr<BaseAI>(new BaseAI(Entity::Side::Right, sf::IntRect(0, 0, winSize.x, winSize.y),
												mBaseTexture, mUnitTextures, mUnitData, mSoundPlayer));

	mMusicPlayer.setVolume(50.f);
	mMusicPlayer.play(MusicID::Soundtrack);
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

	mBackgroundTexture.loadFromFile("Assets/Textures/Background.png");
	mBaseTexture.loadFromFile("Assets/Textures/Base.png");

	mMusicPlayer.loadTheme(MusicID::Soundtrack, "Assets/Music/Soundtrack.wav");
	mSoundPlayer.loadEffect(Unit::SoundID::MageAttack, "Assets/Sounds/MageAttack.wav");
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStateClear();

	mLeftBase->handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	if (mLeftBase->hasUnits() && mRightBase->hasUnits())
	{
		mLeftBase->attack(mRightBase->getFirstUnit());
		mRightBase->attack(mLeftBase->getFirstUnit());
	}
	else {
		mLeftBase->attack(mRightBase.get());
		mRightBase->attack(mLeftBase.get());
	}

	mLeftBase->update(dt);
	mRightBase->update(dt);

	return true;
}

void GameState::draw()
{
	mWindow.draw(mBackground, &mBackgroundTexture);
	mWindow.draw(*mLeftBase);
	mWindow.draw(*mRightBase);
}