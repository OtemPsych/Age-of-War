#include "Turret.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

Turret::Turret(Type type, sf::Vector2f baseSize, const sf::Texture& texture, unsigned damage,
	           float range, sf::Time attackRate)
	: mSprite(texture)
	, mDamage(damage)
	, mRange(range)
	, mAttackRate(attackRate)
{
	setPosition(baseSize.x / 4.f, baseSize.y / 2.f);
}

void Turret::spawnProjectile()
{
	const float radius = 5.f;
	const sf::Vector2f center(5.f, getGlobalBounds().height / 2.f);

	mProjectiles.push_back(sf::VertexArray(sf::Quads, 4));
	mProjectiles.back()[0].position = sf::Vector2f(center.x,          center.y - radius);
	mProjectiles.back()[1].position = sf::Vector2f(center.x + radius, center.y - radius);
	mProjectiles.back()[2].position = sf::Vector2f(center.x + radius, center.y + radius);
	mProjectiles.back()[3].position = sf::Vector2f(center.x,          center.y + radius);

	mProjectiles.back()[0].color = sf::Color::Red;
	mProjectiles.back()[1].color = sf::Color::Red;
	mProjectiles.back()[2].color = sf::Color::Red;
	mProjectiles.back()[3].color = sf::Color::Red;
}

void Turret::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mSprite, states);
	for (const auto& projectile : mProjectiles)
		target.draw(projectile, states);
}

void Turret::attack(Unit& unit)
{
	sf::Vector2f distanceVec = unit.getPosition() - getPosition();
	if (pyro::math::getHypotenuse(distanceVec) <= mRange)
	{
		distanceVec = pyro::math::normalizeVector(distanceVec);
		setRotation(pyro::math::toDegrees(std::atan2f(distanceVec.y, distanceVec.x)));

		if (mAttackRate.current >= mAttackRate.original)
		{
			spawnProjectile();
			mAttackRate.current = sf::Time::Zero;
		}
	}

	if (!mProjectiles.empty() && unit.getGlobalBounds().contains(getTransform().transformPoint(mProjectiles.front()[3].position)))
	{
		unit.reduceHealth(mDamage);
		mProjectiles.erase(mProjectiles.begin());
	}
}

void Turret::update(sf::Time dt)
{
	mAttackRate.current += dt;
}

sf::FloatRect Turret::getGlobalBounds() const
{
	return getTransform().transformRect(mSprite.getGlobalBounds());
}