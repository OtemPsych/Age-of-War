#include "Base.h"

#include <SFML/Graphics/RenderTarget.hpp>

Base::Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	       const pyro::TextureHolder<Unit::Type>& textures,
		   std::vector<gStruct::UnitData>& data,
		   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Entity(side, Entity::Type::Base, 1500, baseTexture)
	, mTextureHolder(textures)
	, mSoundPlayer(soundPlayer)
	, mSpawnBar(getGlobalBounds(), true, sf::Color(153, 77, 0))
	, mUnitTypeToSpawn(-1)
	, mUnitData(data)
	, mGold(200)
{
	if (side == Side::Left)
		setPosition(getGlobalBounds().width / 2.f + 20.f, 0.7f * worldBounds.height);
	else {
		setPosition(worldBounds.width - getGlobalBounds().width / 2.f - 20.f, 0.7f * worldBounds.height);
		mSpawnBar.scale(-1.f, 1.f);
	}
}

Base::~Base()
{
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

	target.draw(mSpawnBar, states.transform *= getTransform());

	for (const auto& turret : mTurrets)
		target.draw(turret, states);
}

void Base::handleUnitSpawn(Unit::Type type)
{
	if (mGold >= mUnitData[type].cost && mSpawnBar.spawnNewUnit(gui::UnitQueue::UnitData(type, mUnitData[type].spawn)))
	{
		modifyGold(-mUnitData[type].cost);
		mUnitTypeToSpawn = static_cast<sf::Int16>(type);
	}
}

void Base::attack(std::shared_ptr<Unit>& otherUnit)
{
	for (auto& unit : mUnits)
	{
		unit->attack(*otherUnit);
		if (otherUnit->isDestroyable())
			break;
	}

	for (auto& turret : mTurrets)
	{
		turret.attack(*otherUnit);
		if (otherUnit->isDestroyable())
			break;
	}

	if (otherUnit->isDestroyable())
	{
		modifyGold(otherUnit->getRewardMoney());
		for (auto& unit : mUnits)
			unit->stopAttacking();
	}
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

	for (auto& turret : mTurrets)
		turret.update(dt);

	if (!mUnits.empty() && mUnits.front()->isDestroyable())
		mUnits.pop_front();
}

void Base::modifyGold(int amount)
{
	mGold += amount;
}

sf::Packet& operator<<(sf::Packet& packet, Base& base)
{
	if (base.mUnitTypeToSpawn > -1) 
	{
		assert(packet << base.mUnitTypeToSpawn);
		base.mUnitTypeToSpawn = -1;
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Base& base)
{
	if (!packet.endOfPacket()) 
	{
		assert(packet >> base.mUnitTypeToSpawn);
		base.handleUnitSpawn(static_cast<Unit::Type>(base.mUnitTypeToSpawn));
	}

	return packet;
}