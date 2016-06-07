#include "Turret.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

Turret::Turret(Side side, sf::Vector2f baseSize, gStruct::TurretData& data,
	const pyro::TextureHolder<TurretType>& textures)
	: Entity(side, EntityType::Turret, textures.get(static_cast<TurretType>(data.turretType)))
	, mTurretType(static_cast<TurretType>(data.turretType))
	, mDamage(data.damage)
	, mRange(data.range)
	, mAttackRate(data.rate)
	, mProjectileSpeed(data.projectileSpeed)
{
	scale(data.scale, data.scale);
}

void Turret::spawnProjectile()
{
	const sf::Vector2f size(5.f, 3.f);
	const sf::FloatRect gBounds(getGlobalBounds());

	sf::VertexArray vert(sf::Quads);
	vert.append(sf::Vertex(sf::Vector2f(   0.f, -size.y), sf::Color::Red));
	vert.append(sf::Vertex(sf::Vector2f(size.x, -size.y), sf::Color::Red));
	vert.append(sf::Vertex(sf::Vector2f(size.x,  size.y), sf::Color::White));
	vert.append(sf::Vertex(sf::Vector2f(   0.f,  size.y), sf::Color::Red));

	mProjectiles.push_back(std::move(vert));
}

void Turret::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RenderStates projState = states.transform * getTransform();
	for (const auto& projectile : mProjectiles)
		target.draw(projectile, projState);

	Entity::draw(target, states);
}

void Turret::attack(Unit& unit)
{
	if (mAttackRate.current >= mAttackRate.original)
	{
		if (mSide == Side::Ally)
		{
			float turretStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
			float unitStartPoint = unit.getPosition().x - unit.getGlobalBounds().width / 2.f;

			if (turretStartPoint + mRange >= unitStartPoint)
			{
				sf::Vector2f distanceVec(unit.getPosition() - getPosition());
				mSprite.setRotation(pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

				spawnProjectile();
				mAttackRate.current = sf::Time::Zero;
			}
		}
		else {
			float turretStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
			float unitStartPoint = unit.getPosition().x + unit.getGlobalBounds().width / 2.f;

			if (turretStartPoint - mRange <= unitStartPoint)
			{
				sf::Vector2f distanceVec(getPosition() - unit.getPosition());
				mSprite.setRotation(pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

				spawnProjectile();
				mAttackRate.current = sf::Time::Zero;
			}
		}
	}

	if (!mProjectiles.empty() && unit.getGlobalBounds().contains(getTransform().transformPoint(mProjectiles.front()[0].position)))
	{
		unit.receiveDamage(mDamage);
		mProjectiles.erase(mProjectiles.begin());
	}

	mEnemyPosition = unit.getPosition();
}

void Turret::update(sf::Time dt)
{
	mAttackRate.current += dt;

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

	if (!mProjectiles.empty() && pyro::math::getHypotenuse(mProjectiles.front()[0].position) > mRange * 1.5f)
		mProjectiles.erase(mProjectiles.begin());
}