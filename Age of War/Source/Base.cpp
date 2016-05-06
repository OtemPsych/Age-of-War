#include "Base.h"

#include <SFML/Graphics/RenderTarget.hpp>

Base::Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	       const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	       std::vector<gStruct::UnitData>& unitData,
		   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		   std::vector<gStruct::TurretData>& turretData,
	       pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: HealthEntity(side, EntityType::Base, 1500, baseTexture)
	, mSpawnBar(getGlobalBounds(), true, sf::Color(153, 77, 0))
	, mUnitTypeToSpawn(-1)
	, mUnitTextures(unitTextures)
	, mSoundPlayer(soundPlayer)
	, mUnitData(unitData)
	, mTurretTextures(turretTextures)
	, mTurretData(turretData)
	, mGold(200)
{
	if (side == Side::Ally)
		setPosition(getGlobalBounds().width / 2.f + 20.f, 0.7f * worldBounds.height);
	else {
		setPosition(worldBounds.width - getGlobalBounds().width / 2.f - 20.f, 0.7f * worldBounds.height);
		mSpawnBar.scale(-1.f, 1.f);
	}
}

Base::~Base()
{
}

void Base::handleUnitSpawn(Unit::UnitType type)
{
	if (mGold >= mUnitData[type].cost && mSpawnBar.spawnNewUnit(gui::UnitQueue::UnitData(type, mUnitData[type].spawn)))
	{
		modifyGold(-mUnitData[type].cost);
		mUnitTypeToSpawn = static_cast<sf::Int16>(type);
	}
}

void Base::spawnUnit()
{
	Unit::UnitType type(mSpawnBar.getUnitTypeSpawning());
	if (mUnitData[type].generalUnitType == Unit::GeneralUnitType::Melee)
		mUnits.emplace_back(new Unit(mSide, mUnitData[type], mUnitTextures, mSoundPlayer));
	else
		mUnits.emplace_back(new RangedUnit(mSide, mUnitData[type], mUnitTextures, mSoundPlayer));

	mUnits.back()->setPosition(getPosition().x,
		getPosition().y + getGlobalBounds().height / 2.f - mUnits.back()->getGlobalBounds().height / 2.f);
}

void Base::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	HealthEntity::draw(target, states);

	for (const auto& unit : mUnits)
		target.draw(*unit, states);

	for (const auto& turret : mTurrets)
		target.draw(turret, states);

	target.draw(mSpawnBar, states.transform *= getTransform());
}

void Base::attack(Unit& enemyUnit)
{
	for (auto& unit : mUnits)
	{
		unit->attack(enemyUnit);
		if (enemyUnit.isDestroyable())
			break;
	}

	for (auto& turret : mTurrets)
	{
		turret.attack(enemyUnit);
		if (enemyUnit.isDestroyable())
			break;
	}

	if (enemyUnit.isDestroyable())
	{
		modifyGold(enemyUnit.getGoldReward());
		for (auto& unit : mUnits)
			unit->stopAttacking();
	}
}

void Base::attack(Base& enemyBase)
{
	for (auto& unit : mUnits)
	{
		unit->attack(enemyBase);
	}
}

void Base::update(sf::Time dt)
{
	for (auto& itr = mUnits.begin(); itr != mUnits.end(); itr++)
	{
		auto& nextItr = std::next(itr);
		if (nextItr != mUnits.end() && (*itr)->getGlobalBounds().intersects((*nextItr)->getGlobalBounds()))
			(*nextItr)->startMovement(false);
	}

	if (mSpawnBar.update(dt))
		spawnUnit();

	for (auto& unit : mUnits)
		unit->update(dt);

	for (auto& turret : mTurrets)
		turret.update(dt);

	if (!mUnits.empty() && mUnits.front()->isDestroyable())
		mUnits.erase(mUnits.begin());
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
		base.handleUnitSpawn(static_cast<Unit::UnitType>(base.mUnitTypeToSpawn));
	}

	return packet;
}