#include "Unit.h"
#include "Base.h"

#include <PYRO/Math.h>

Unit::AttackRate::AttackRate(sf::Time rate)
	: original(rate)
	, current(rate)
{
}

Unit::Unit(Side side, UnitData& data, const pyro::TextureHolder<Unit::Type>& textureHolder,
		   pyro::SoundPlayer<SoundID>& soundPlayer)
	: Entity(side, Entity::Type::Unit, data.health, textureHolder.get(data.type), data.walkRects[0])
	, mType(data.type)
	, mDamage(data.damage)
	, mAttackRange(data.range)
	, mAttackRate(data.rate)
	, mSpeed(data.speed)
	, mReward(125u * data.cost / 100u)
	, mMoving(true)
	, mAttacking(false)
	, mWalkingAnimation(mSprite, data.walkRects, sf::seconds(0.75f), true)
	, mAttackAnimation(mSprite, data.attackRects, data.rate, false)
	, mSoundPlayer(soundPlayer)
{
	scale(data.scale, data.scale);
}

Unit::~Unit()
{
}

bool Unit::canAttackTarget(Entity& entity)
{
	if (mAttackRate.current == mAttackRate.original)
	{
		float unitStartPoint, entityStartPoint;

	/*	unitStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
		entityStartPoint = entity.getPosition().x - entity.getGlobalBounds().width / 2.f;*/
		if (mSide == Side::Left)
		{
			unitStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
			entityStartPoint = entity.getPosition().x - entity.getGlobalBounds().width / 2.f;
		}
		else {
			unitStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
			entityStartPoint = entity.getPosition().x + entity.getGlobalBounds().width / 2.f;
		}

		if (abs(entityStartPoint - unitStartPoint) <= mAttackRange)
			return true;
		else
			mMoving = true;
	}

	return false;
}

void Unit::attack(Entity& entity)
{
	mAttacking = canAttackTarget(entity);

	if (!mAttackAnimation.isAnimationOngoing())
	{
		entity.reduceHealth(mDamage);
		mAttackRate.current = sf::Time::Zero;
		mAttackAnimation.restart();
		//if (entity.isDestroyable())
		//	mAttacking = false;
	}
}

void Unit::update(sf::Time dt)
{
	if (mAttackRate.current < mAttackRate.original)
		if (mAttackRate.current + dt >= mAttackRate.original)
			mAttackRate.current = mAttackRate.original;
		else
			mAttackRate.current += dt;

	if (!mAttacking && mMoving)
	{
		mWalkingAnimation.update(dt);
		if (mSide == Side::Left)
			move(mSpeed * dt.asSeconds(), 0.f);
		else
			move(-(mSpeed * dt.asSeconds()), 0.f);
	}
	else if (mAttacking)
		mAttackAnimation.update(dt);
}

std::vector<Unit::UnitData> Unit::initializeUnitData()
{
	std::vector<UnitData> data(Unit::TypeCount);

	data[Unit::Mage].type   = Unit::Mage;
	data[Unit::Mage].health = 75;
	data[Unit::Mage].damage = 25;
	data[Unit::Mage].range  = 100.f;
	data[Unit::Mage].rate   = sf::seconds(0.5f);
	data[Unit::Mage].speed  = 50.f;
	data[Unit::Mage].spawn  = sf::seconds(2.f);
	data[Unit::Mage].cost	= 25;
	data[Unit::Mage].scale  = 0.25f;
	data[Unit::Mage].walkRects.push_back(sf::IntRect(0, 0, 93, 134));
	data[Unit::Mage].walkRects.push_back(sf::IntRect(107, 4, 114, 128));
	data[Unit::Mage].attackRects.push_back(sf::IntRect(107, 4, 114, 128));
	data[Unit::Mage].attackRects.push_back(sf::IntRect(232, 5, 121, 123));
	data[Unit::Mage].attackRects.push_back(sf::IntRect(380, 5, 121, 123));

	data[Unit::Knight].type   = Unit::Knight;
	data[Unit::Knight].health = 180;
	data[Unit::Knight].damage = 45;
	data[Unit::Knight].range  = 5.f;
	data[Unit::Knight].rate   = sf::seconds(0.75f);
	data[Unit::Knight].speed  = 65.f;
	data[Unit::Knight].spawn  = sf::seconds(3.5f);
	data[Unit::Knight].cost   = 50;
	data[Unit::Knight].scale  = 0.25f;
	data[Unit::Knight].walkRects.push_back(sf::IntRect(66, 2, 210, 252));
	data[Unit::Knight].walkRects.push_back(sf::IntRect(318, 11, 260, 246));
	data[Unit::Knight].attackRects.push_back(sf::IntRect(643, 15, 306, 236));
	data[Unit::Knight].attackRects.push_back(sf::IntRect(992, 9, 315, 226));
	data[Unit::Knight].attackRects.push_back(sf::IntRect(1335, 10, 303, 260));
	data[Unit::Knight].attackRects.push_back(sf::IntRect(1699, 11, 272, 256));

	data[Unit::Destroyer].type   = Unit::Destroyer;
	data[Unit::Destroyer].health = 450;
	data[Unit::Destroyer].damage = 85;
	data[Unit::Destroyer].range  = 5.f;
	data[Unit::Destroyer].rate   = sf::seconds(0.75f);
	data[Unit::Destroyer].speed  = 50.f;
	data[Unit::Destroyer].spawn  = sf::seconds(5.f);
	data[Unit::Destroyer].cost   = 200;
	data[Unit::Destroyer].scale  = 1.f;
	data[Unit::Destroyer].walkRects.push_back(sf::IntRect(4, 15, 211, 169));
	data[Unit::Destroyer].walkRects.push_back(sf::IntRect(287, 10, 207, 167));
	data[Unit::Destroyer].attackRects.push_back(sf::IntRect(874, 0, 172, 157));

	for (unsigned i = 0; i < data.size(); i++)
	{
		for (int j = data[i].walkRects.size() - 2; j >= 0; j--)
			data[i].walkRects.push_back(data[i].walkRects[j]);

		for (int j = data[i].attackRects.size() - 2; j >= 0; j--)
			data[i].attackRects.push_back(data[i].attackRects[j]);
	}

	return data;
}