#include "RangedUnit.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

RangedUnit::RangedUnit(Side side, sf::Font& font, gStruct::UnitData& data,
	                   const pyro::TextureHolder<UnitType>& textures, pyro::SoundPlayer<SoundID>& soundPlayer)
	: Unit(side, font, data, textures, soundPlayer)
	, mProjectileSpeed(data.rangedData->projectileSpeed)
	, mSpawnProjectile(data.rangedData->spawnProjectile)
{
}

void RangedUnit::handleAttackAnimation(HealthEntity& enemy)
{
	if (!mAttackAnimation.isAnimationOngoing())
	{
		sf::FloatRect gBounds(getGlobalBounds());
		mSpawnProjectile(mProjectiles, sf::Vector2f(gBounds.width, gBounds.height));

		if (mUnitType < static_cast<unsigned short>(Unit::SoundID::TypeCount))
			mSoundPlayer.play(static_cast<Unit::SoundID>(mUnitType), getPosition(), 10.f);

		mAttackAnimation.restart();
	}
}

void RangedUnit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Unit::draw(target, states);

	states.transform *= getTransform();
	for (const auto& projectile : mProjectiles)
		target.draw(projectile, states);
}

void RangedUnit::attack(HealthEntity& enemy)
{
	Unit::attack(enemy);

	for (unsigned i = 0; i < mProjectiles.size(); i++)
		if (enemy.getGlobalBounds().contains(getTransform().transformPoint(mProjectiles[i][0].position)))
		{
			enemy.receiveDamage(mDamage);
			addDamageDisplay(enemy.getGlobalBounds());
			mProjectiles.erase(mProjectiles.begin() + i);
		}

	mEnemyPosition = enemy.getPosition();
}

void RangedUnit::update(sf::Time dt)
{
	for (auto& projectile : mProjectiles)
	{
		sf::Vector2f pos(getPosition());
		float distance = pyro::math::getHypotenuse(mEnemyPosition - pos);

		sf::Vector2f velocity(mProjectileSpeed * (mEnemyPosition.x - pos.x) / distance * dt.asSeconds(),
			                  mProjectileSpeed * (mEnemyPosition.y - pos.y) / distance * dt.asSeconds());
		if (mSide == Side::Enemy)
			velocity = sf::Vector2f(-velocity.x, velocity.y);

		for (unsigned i = 0; i < projectile.getVertexCount(); i++)
			projectile[i].position += velocity;
	}

	Unit::update(dt);
}