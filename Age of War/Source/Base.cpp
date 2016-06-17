#include "Base.h"
#include "RangedUnit.h"

#include <SFML/Graphics/RenderTarget.hpp>

Base::Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	       const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	       std::vector<gStruct::UnitData>& unitData,
		   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		   std::vector<gStruct::TurretData>& turretData,
	       pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: HealthEntity(side, EntityType::Base, 1500, baseTexture)
	, mSpawnBar(getGlobalBounds(), true, sf::Color(153, 77, 0))
	, mMPUnitType(-1)
	, mMPTurretInfo(std::make_pair(-1, -1))
	, mTurretTypeToSpawn(-1)
	, mUnitTextures(unitTextures)
	, mSoundPlayer(soundPlayer)
	, mUnitData(unitData)
	, mTurretTextures(turretTextures)
	, mTurretData(turretData)
	, mGold(200)
{
	if (side == Side::Ally) {
		setPosition(getGlobalBounds().width / 2.f + 20.f, 0.7f * worldBounds.height);
	} else {
		setPosition(worldBounds.width - getGlobalBounds().width / 2.f - 20.f, 0.7f * worldBounds.height);
		mSpawnBar.scale(-1.f, 1.f);
	}

	const sf::FloatRect gBounds(getGlobalBounds());
	mTurretWindowRects[0] = sf::FloatRect( 18.f - gBounds.width / 2.f, 103.f - gBounds.height / 2.f, 43.f, 44.f);
	mTurretWindowRects[1] = sf::FloatRect( 67.f - gBounds.width / 2.f, 103.f - gBounds.height / 2.f, 43.f, 44.f);
	mTurretWindowRects[2] = sf::FloatRect(117.f - gBounds.width / 2.f, 103.f - gBounds.height / 2.f, 43.f, 44.f);
}

Base::~Base()
{
}

void Base::handleTurretSpawn(Turret::TurretType type, int turretIndicator)
{
	if (turretIndicator == -1)
		return;

	if (mGold >= mTurretData[type].cost) 
	{
		for (const auto& turret : mTurrets)
			if (turret.second == turretIndicator)
				return;

		modifyGold(-mTurretData[type].cost);
		mTurrets.emplace_back(std::make_pair(Turret(mSide, sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height),
			                                 mTurretData[type], mTurretTextures), turretIndicator));
		mTurrets.back().first.setPosition(getPosition().x + mTurretWindowRects[turretIndicator].left + mTurretWindowRects[turretIndicator].width / 2.f,
			                              getPosition().y + mTurretWindowRects[turretIndicator].top + mTurretWindowRects[turretIndicator].height / 2.f);

		mTurretTypeToSpawn = static_cast<sf::Int16>(type);
		mMPTurretInfo.first = mTurretTypeToSpawn;
		mMPTurretInfo.second = turretIndicator;
	}
}

void Base::handleUnitSpawn(Unit::UnitType type)
{
	if (mGold >= mUnitData[type].cost && mSpawnBar.spawnNewUnit(gui::UnitQueue::UnitData(type, mUnitData[type].spawn)))
	{
		modifyGold(-mUnitData[type].cost);
		mMPUnitType = static_cast<sf::Int16>(type);
	}
}

void Base::spawnUnit()
{
	Unit::UnitType type(mSpawnBar.getUnitTypeSpawning());
	if (mUnitData[type].generalUnitType == Unit::GeneralUnitType::Melee)
		mUnits.emplace_back(new Unit(mSide, mUnitData[type], mUnitTextures, mSoundPlayer));
	else
		mUnits.emplace_back(new RangedUnit(mSide, mUnitData[type], mUnitTextures, mSoundPlayer));

	const sf::Vector2f& pos(getPosition());
	const sf::FloatRect gBounds(getGlobalBounds());
	mUnits.back()->setPosition(mSide == Side::Ally ? pos.x + gBounds.width / 2.f : pos.x - gBounds.width / 2.f,
		pos.y + gBounds.height / 2.f - mUnits.back()->getGlobalBounds().height / 2.f);
}

void Base::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	HealthEntity::draw(target, states);

	for (const auto& turret : mTurrets)
		target.draw(turret.first, states);

	for (const auto& unit : mUnits)
		target.draw(*unit, states);

	target.draw(mSpawnBar, states.transform *= getTransform());
}

void Base::attack(Unit& enemyUnit)
{
	for (auto& itr = mUnits.begin(); itr != mUnits.end(); itr++)
		if ((*itr)->getGeneralUnitType() == Unit::GeneralUnitType::Ranged || itr == mUnits.begin()) {
			(*itr)->attack(enemyUnit);
			if (enemyUnit.isDestroyable())
				break;
		}

	for (auto& turret : mTurrets)
	{
		turret.first.attack(enemyUnit);
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
	for (auto& itr = mUnits.begin(); itr != mUnits.end(); itr++)
		if ((*itr)->getGeneralUnitType() == Unit::GeneralUnitType::Ranged || itr == mUnits.begin()) {
			(*itr)->attack(enemyBase);
			if (enemyBase.isDestroyable())
				break;
		}
}

void Base::update(sf::Time dt)
{
	for (auto& itr = mUnits.begin(); itr != mUnits.end(); itr++)
	{
		auto& nextItr = std::next(itr);
		if (nextItr != mUnits.end())
		{
			sf::FloatRect itrGBounds((*itr)->getGlobalBounds());
			sf::FloatRect nextItrGBounds((*nextItr)->getGlobalBounds());
			(*nextItr)->startMovement(!itrGBounds.intersects(nextItrGBounds));
		}
	}

	if (mSpawnBar.update(dt))
		spawnUnit();

	for (auto& unit : mUnits)
		unit->update(dt);

	for (auto& turret : mTurrets)
		turret.first.update(dt);

	if (!mUnits.empty() && mUnits.front()->isDestroyable())
		mUnits.erase(mUnits.begin());
}

void Base::modifyGold(int amount)
{
	mGold += amount;
}

sf::Packet& operator<<(sf::Packet& packet, Base& base)
{
	if (base.mMPUnitType > -1) {
		assert(packet << static_cast<sf::Uint8>(Base::PacketTypeID::Unit));
		assert(packet << base.mMPUnitType);
		base.mMPUnitType = -1;
	}
	if (base.mMPTurretInfo.first > -1) {
		assert(packet << static_cast<sf::Uint8>(Base::PacketTypeID::Turret));
		assert(packet << base.mMPTurretInfo.first);

		switch (base.mMPTurretInfo.second) {
		case sf::Int8(0):
			assert(packet << sf::Int8(2));
			break;
		case sf::Int8(2):
			assert(packet << sf::Int8(0));
			break;
		default:
			assert(packet << sf::Int8(1));
		}

		base.mMPTurretInfo.first = -1;
		base.mMPTurretInfo.second = -1;
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Base& base)
{
	if (!packet.endOfPacket()) {
		sf::Uint8 packetTypeID;
		assert(packet >> packetTypeID);
		if (static_cast<Base::PacketTypeID>(packetTypeID) == Base::PacketTypeID::Unit) {
			assert(packet >> base.mMPUnitType);
			base.handleUnitSpawn(static_cast<Unit::UnitType>(base.mMPUnitType));
		}
		else {
			assert(packet >> base.mMPTurretInfo.first);
			assert(packet >> base.mMPTurretInfo.second);
			base.handleTurretSpawn(static_cast<Turret::TurretType>(base.mMPTurretInfo.first), base.mMPTurretInfo.second);
		}
	}

	return packet;
}