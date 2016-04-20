#include "Entity.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Health::Health(unsigned health)
	: original(health)
	, current(health)
{
}

Entity::Entity(Side side, Type type, unsigned health,
			   const sf::Texture& texture, sf::IntRect textureRect)
	: mSprite(texture, textureRect == sf::IntRect() ? sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y) : textureRect)
	, mSide(side)
	, mHealth(health)
	, mHealthBar(mSprite.getGlobalBounds(), type == Type::Unit, side == Side::Left ? sf::Color(0, 230, 0) : sf::Color(179, 0, 0))
	, mIsDestroyable(false)
{
	if (mSide == Side::Right) 
	{
		sf::IntRect rect(mSprite.getTextureRect());
		mSprite.setTextureRect(sf::IntRect(rect.width, rect.top, -rect.width, rect.height));
	}
}

Entity::~Entity()
{
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mSprite, states);
	target.draw(mHealthBar, states);
}

void Entity::reduceHealth(unsigned damage)
{
	if (mHealth.current > damage)
		mHealth.current -= damage;
	else
	{
		mHealth.current = 0;
		mIsDestroyable = true;
	}

	mHealthBar.update(mHealth.original, mHealth.current);
}

void Entity::update(sf::Time dt)
{
}

sf::FloatRect Entity::getGlobalBounds() const
{
	sf::FloatRect entityBounds(getTransform().transformRect(mSprite.getGlobalBounds()));
	sf::Vector2f pos(getPosition());

	return sf::FloatRect(pos.x, pos.y, entityBounds.width, entityBounds.height);
}