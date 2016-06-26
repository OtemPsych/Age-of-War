#include "Turret.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

Turret::Turret(Side side, sf::Vector2f baseSize, sf::Font& font, gStruct::TurretData& data,
	const pyro::TextureHolder<TurretType>& textures)
	: AttackEntity(side, EntityType::Turret, data.range, data.rate, data.damage, font,
	               textures.get(static_cast<TurretType>(data.turretType)))
	, mTurretType(static_cast<TurretType>(data.turretType))
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

	AttackEntity::draw(target, states);
}

void Turret::attack(HealthEntity& enemy)
{
	if (mAttackRate.current >= mAttackRate.original)
	{
		if (mSide == Side::Ally)
		{
			float turretStartPoint = getPosition().x + getGlobalBounds().width / 2.f;
			float unitStartPoint = enemy.getPosition().x - enemy.getGlobalBounds().width / 2.f;

			if (turretStartPoint + mAttackRange >= unitStartPoint)
			{
				sf::Vector2f distanceVec(enemy.getPosition() - getPosition());
				mSprite.setRotation(pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

				spawnProjectile();
				mAttackRate.current = sf::Time::Zero;
			}
		}
		else {
			float turretStartPoint = getPosition().x - getGlobalBounds().width / 2.f;
			float unitStartPoint = enemy.getPosition().x + enemy.getGlobalBounds().width / 2.f;

			if (turretStartPoint - mAttackRange <= unitStartPoint)
			{
				sf::Vector2f distanceVec(getPosition() - enemy.getPosition());
				mSprite.setRotation(-pyro::math::toDegrees(std::atan2(distanceVec.y, distanceVec.x)));

				spawnProjectile();
				mAttackRate.current = sf::Time::Zero;
			}
		}
	}

	if (!mProjectiles.empty() && enemy.getGlobalBounds().contains(getTransform().transformPoint(mProjectiles.front()[0].position)))
	{
		enemy.receiveDamage(mDamage);
		mProjectiles.erase(mProjectiles.begin());
	}

	mEnemyPosition = enemy.getPosition();
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

	if (!mProjectiles.empty() && pyro::math::getHypotenuse(mProjectiles.front()[0].position) > mAttackRange * 1.5f)
		mProjectiles.erase(mProjectiles.begin());
}