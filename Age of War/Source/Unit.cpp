#include "Unit.h"
#include "Base.h"

#include <PYRO/Math.h>

Unit::Unit(Side side, gStruct::UnitData& data, const pyro::TextureHolder<Unit::Type>& textureHolder,
	       pyro::SoundPlayer<SoundID>& soundPlayer)
	: Entity(side, Entity::Type::Unit, data.health, textureHolder.get(static_cast<Unit::Type>(data.type)),
			 data.walkRects[0])
	, mType(static_cast<Unit::Type>(data.type))
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
		if (mSide == Side::Left)
		{
			float unitStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
			float entityStartPoint = entity.getPosition().x - entity.getGlobalBounds().width / 2.f;

			if (unitStartPoint + mAttackRange >= entityStartPoint)
				return true;
		}
		else {
			float unitStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
			float entityStartPoint = entity.getPosition().x + entity.getGlobalBounds().width / 2.f;

			if (unitStartPoint - mAttackRange <= entityStartPoint)
				return true;
		}
			
		mMoving = true;
	}

	return false;
}

void Unit::attack(Entity& entity)
{
	if (!mAttacking && canAttackTarget(entity))
		mAttacking = true;

	if (!mAttackAnimation.isAnimationOngoing())
	{
		entity.reduceHealth(mDamage);
		mAttackRate.current = sf::Time::Zero;
		mAttackAnimation.restart();
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