#include "HealthEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

HealthEntity::HealthEntity(Side side, EntityType entityType, unsigned short health, const sf::Texture& texture)
	: Entity(side, entityType, texture)
	, mHealth(health)
	, mHealthBar(getGlobalBounds(), entityType == EntityType::Unit, side == Side::Ally ? sf::Color(0, 230, 0) : sf::Color(179, 0, 0))
	, mIsDestroyable(false)
{
	if (side == Side::Enemy && entityType == EntityType::Unit)
		mHealthBar.rotate(180.f);
}

HealthEntity::HealthEntity(Side side, EntityType entityType, unsigned short health, const sf::Texture& texture, sf::IntRect rect)
	: Entity(side, entityType, texture, rect)
	, mHealth(health)
	, mHealthBar(getGlobalBounds(), entityType == EntityType::Unit, side == Side::Ally ? sf::Color(0, 230, 0) : sf::Color(179, 0, 0))
	, mIsDestroyable(false)
{
	if (side == Side::Enemy && entityType == EntityType::Unit)
		mHealthBar.rotate(180.f);
}

HealthEntity::~HealthEntity()
{
}

void HealthEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	target.draw(mHealthBar, states.transform *= getTransform());
}

void HealthEntity::receiveDamage(unsigned short damage)
{
	if (mHealth.current > damage)
		mHealth.current -= damage;
	else {
		mHealth.current = 0;
		mIsDestroyable = true;
	}

	mHealthBar.update(mHealth.original, mHealth.current);
}