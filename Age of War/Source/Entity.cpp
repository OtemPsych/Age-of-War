#include "Entity.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Entity::Entity(Side side, Type type, unsigned health,
			   const sf::Texture& texture, sf::IntRect textureRect)
	: mSprite(texture, textureRect == sf::IntRect() ? sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y) : textureRect)
	, mSide(side)
	, mHealth(health)
	, mHealthBar(mSprite.getGlobalBounds(), type == Type::Unit, side == Side::Left ? sf::Color(0, 230, 0) : sf::Color(179, 0, 0))
	, mIsDestroyable(false)
{
	sf::FloatRect lBounds(mSprite.getLocalBounds());
	mSprite.setOrigin(lBounds.width / 2.f, lBounds.height / 2.f);

	if (mSide == Side::Right) 
	{
		scale(-1.f, 1.f);
		if (type == Type::Unit)
			mHealthBar.rotate(180.f);
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
	else {
		mHealth.current = 0;
		mIsDestroyable = true;
	}

	mHealthBar.update(mHealth.original, mHealth.current);
}

sf::FloatRect Entity::getGlobalBounds() const
{
	return getTransform().transformRect(mSprite.getGlobalBounds());
}