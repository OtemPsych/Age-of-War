#include "Unit.h"
#include "Base.h"

#include <PYRO/Math.h>

Unit::Unit(Side side, gStruct::UnitData& data, const pyro::TextureHolder<UnitType>& unitTextures,
	       pyro::SoundPlayer<SoundID>& soundPlayer)
	: HealthEntity(side, EntityType::Unit, data.health, unitTextures.get(static_cast<UnitType>(data.unitType)),
			       data.walkRects[0])
	, mUnitType(static_cast<UnitType>(data.unitType))
	, mDamage(data.damage)
	, mAttackRange(data.range)
	, mAttackRate(data.rate)
	, mSpeed(data.speed)
	, mGoldReward(125u * data.cost / 100u)
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

bool Unit::canAttackTarget(HealthEntity& enemy)
{
	if (mAttackRate.current == mAttackRate.original)
	{
		if (mSide == Side::Ally)
		{
			float unitStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
			float entityStartPoint = enemy.getPosition().x - enemy.getGlobalBounds().width / 2.f;

			if (unitStartPoint + mAttackRange >= entityStartPoint)
				return true;
		}
		else {
			float unitStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
			float entityStartPoint = enemy.getPosition().x + enemy.getGlobalBounds().width / 2.f;

			if (unitStartPoint - mAttackRange <= entityStartPoint)
				return true;
		}
			
		mMoving = true;
	}

	return false;
}

void Unit::attack(HealthEntity& enemy)
{
	if (!mAttacking && canAttackTarget(enemy))
		mAttacking = true;

	if (!mAttackAnimation.isAnimationOngoing())
	{
		mSoundPlayer.play(static_cast<Unit::SoundID>(mUnitType), getPosition(), 15.f);
		enemy.receiveDamage(mDamage);
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
		if (mSide == Side::Ally)
			move(mSpeed * dt.asSeconds(), 0.f);
		else
			move(-(mSpeed * dt.asSeconds()), 0.f);
	}
	else if (mAttacking)
		mAttackAnimation.update(dt);
}