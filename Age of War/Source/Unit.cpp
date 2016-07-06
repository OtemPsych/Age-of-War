#include "Unit.h"
#include "Base.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

Unit::Unit(Side side, sf::Font& damageDisplayFont, data::UnitData& data,
	       const pyro::TextureHolder<UnitType>& unitTextures, pyro::SoundPlayer<SoundID>& soundPlayer)
	: HealthEntity(side, EntityType::Unit, data.health.value.current, unitTextures.get(static_cast<UnitType>(data.unitType)),
			       data.walkRects.front().first)
	, mAttackRange(data.range.value.current)
	, mAttackRate(data.rate.value.current)
	, mSpeed(data.speed)
	, mDamageDisplays(sf::Vector2f(0.f, 30.f), sf::Vector2f(0.f, -1.8f), sf::seconds(0.75f), damageDisplayFont)
	, mMoving(true)
	, mWalkingAnimation(mSprite, data.walkRects, sf::seconds(0.65f), true)
	, mGeneralUnitType(static_cast<GeneralUnitType>(data.generalUnitType))
	, mUnitType(static_cast<UnitType>(data.unitType))
	, mDamage(data.damage.value.current)
	, mSoundPlayer(soundPlayer)
	, mAttacking(false)
	, mAttackAnimation(mSprite, data.attackRects, data.rate.value.current, false)
{
	scale(data.scale, data.scale);
}

Unit::~Unit()
{
}

bool Unit::enemyInRange(HealthEntity& enemy)
{
	const sf::FloatRect gBounds(getGlobalBounds());
	const sf::FloatRect enemyGBounds(enemy.getGlobalBounds());

	if (mSide == Side::Ally) {
		if (gBounds.left + gBounds.width + mAttackRange >= enemyGBounds.left)
			return true;
	}
	else
		if (gBounds.left - mAttackRange <= enemyGBounds.left + enemyGBounds.width)
			return true;

	return false;
}

void Unit::handleAttackAnimation(HealthEntity& enemy)
{
	if (!mAttackAnimation.isAnimationOngoing())
	{
		if (mUnitType < static_cast<unsigned short>(Unit::SoundID::TypeCount))
			mSoundPlayer.play(static_cast<Unit::SoundID>(mUnitType), getPosition(), 10.f);
		enemy.receiveDamage(mDamage);
		mDamageDisplays.addValueDisplay(getGlobalBounds(), enemy.getGlobalBounds(), mDamage);
		mAttackRate.current = sf::Time::Zero;
		mAttackAnimation.restart();
	}
}

void Unit::drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mDamageDisplays, states);
}

void Unit::attack(HealthEntity& enemy)
{
	if (enemyInRange(enemy)) {
		mMoving = false;
		if (mAttackRate.current >= mAttackRate.original)
			mAttacking = true;
	}
	else
		mMoving = true;

	handleAttackAnimation(enemy);
}

void Unit::update(sf::Time dt)
{
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

	mDamageDisplays.update(dt);
}