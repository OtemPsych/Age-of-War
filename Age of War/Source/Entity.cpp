#include "Entity.h"

#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(Side side, EntityType entityType, const sf::Texture& texture)
	: mSide(side)
	, mEntityType(entityType)
	, mSprite(texture)
{
	setup();
}

Entity::Entity(Side side, EntityType entityType, const sf::Texture& texture, sf::IntRect rect)
	: mSide(side)
	, mEntityType(entityType)
	, mSprite(texture, rect)
{
	setup();
}

Entity::~Entity()
{
}

void Entity::setup()
{
	sf::FloatRect lBounds(mSprite.getLocalBounds());
	mSprite.setOrigin(lBounds.width / 2.f, lBounds.height / 2.f);

	if (mSide == Side::Enemy)
		scale(-1.f, 1.f);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states.transform *= getTransform());
}

sf::FloatRect Entity::getGlobalBounds() const
{
	return getTransform().transformRect(mSprite.getGlobalBounds());
}