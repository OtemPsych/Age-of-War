#include "Base.h"

#include <SFML/Graphics/RenderTarget.hpp>

Base::Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	       const pyro::TextureHolder<Unit::Type>& textures,
		   std::vector<Unit::UnitData>& data,
		   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Entity(side, Entity::Type::Base, 1500, baseTexture)
	, mTextureHolder(textures)
	, mSoundPlayer(soundPlayer)
	, mSpawnBar(getGlobalBounds(), true, sf::Color(153, 77, 0))
	, mUnitData(data)
	, mGold(200)
{
	if (side == Side::Left)
		setPosition(getGlobalBounds().width / 2.f + 20.f, 0.75f * worldBounds.height);
	else {
		setPosition(worldBounds.width - getGlobalBounds().width / 2.f - 20.f, 0.75f * worldBounds.height);
		mSpawnBar.scale(-1.f, 1.f);
	}

	mGoldCoinTexture.loadFromFile("Assets/Textures/GoldCoin.png");
	mGoldCoinSprite.setTexture(mGoldCoinTexture);
	mGoldCoinSprite.setPosition(10.f, 10.f);

	mGoldFont.loadFromFile("Assets/Fonts/Gold.ttf");
	mGoldText.setFont(mGoldFont);
	mGoldText.setCharacterSize(45);
	mGoldText.setColor(sf::Color::Black);
	mGoldText.setString(std::to_string(mGold));
	mGoldText.centerOrigin();
	mGoldText.setPosition(mGoldCoinSprite.getGlobalBounds().width + mGoldText.getGlobalBounds().width / 2.f + 15.f,
						  mGoldCoinSprite.getGlobalBounds().height / 2.f);
}

Base::~Base()
{
}

void Base::updateGoldGUI()
{
	mGoldText.setString(std::to_string(mGold));
	mGoldText.centerOrigin();
	mGoldText.setPosition(mGoldCoinSprite.getGlobalBounds().width + mGoldText.getGlobalBounds().width / 2.f + 15.f,
						  mGoldText.getPosition().y);
}

void Base::spawnUnit()
{
	Unit::Type type(mSpawnBar.getUnitTypeSpawning());
	if (type == Unit::Type::Mage)
		mUnits.emplace_back(new Mage(mSide, mUnitData[type], mTextureHolder, mSoundPlayer));
	else
		mUnits.emplace_back(new Unit(mSide, mUnitData[type], mTextureHolder, mSoundPlayer));

	mUnits.back()->setPosition(getPosition().x, getPosition().y + getGlobalBounds().height / 2.f - mUnits.back()->getGlobalBounds().height / 2.f);
}

void Base::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);

	for (const auto& unit : mUnits)
		target.draw(*unit, states);

	if (mSide == Side::Left)
	{
		target.draw(mGoldCoinSprite, states);
		target.draw(mGoldText, states.transform * mGoldCoinSprite.getTransform());
	}

	target.draw(mSpawnBar, states.transform *= getTransform());
}

void Base::handleUnitSpawn(Unit::Type type)
{
	if (mGold >= mUnitData[type].cost && mSpawnBar.spawnNewUnit(type, mUnitData[type].spawn))
	{
		mGold -= mUnitData[type].cost;
		updateGoldGUI();
	}
}

void Base::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Num1)
			handleUnitSpawn(Unit::Type::Mage);
		else if (event.key.code == sf::Keyboard::Num2)
			handleUnitSpawn(Unit::Type::Knight);
		else if (event.key.code == sf::Keyboard::Num3)
			handleUnitSpawn(Unit::Type::Destroyer);
	}
}

void Base::attack(std::unique_ptr<Unit>& otherUnit)
{
	for (auto& unit : mUnits)
	{
		unit->attack(*otherUnit);
		if (otherUnit->isDestroyable())
		{
			mGold += otherUnit->getRewardMoney();
			updateGoldGUI();
			break;
		}
	}

	if (otherUnit->isDestroyable())
		for (auto& unit : mUnits)
			unit->stopAttacking();
}

void Base::attack(Base* otherBase)
{
	for (auto& unit : mUnits)
	{
		unit->attack(*otherBase);
		if (otherBase->isDestroyable())
		{
			
		}
	}
}

void Base::update(sf::Time dt)
{
	for (auto& itr = mUnits.begin(); itr != mUnits.end(); itr++)
	{
		auto& nextItr = std::next(itr);
		if (nextItr != mUnits.end())
			if ((*itr)->getGlobalBounds().intersects((*nextItr)->getGlobalBounds()))
				(*nextItr)->startMovement(false);
	}

	if (mSpawnBar.update(dt))
		spawnUnit();

	for (auto& Unit : mUnits)
		Unit->update(dt);

	if (!mUnits.empty() && mUnits.front()->isDestroyable())
		mUnits.pop_front();
}