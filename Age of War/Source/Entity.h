#ifndef Entity_H_
#define Entity_H_

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	enum class Side { Ally, Enemy };
protected:
	enum class EntityType { Unit, Turret, Base };
protected:
	Side       mSide;
	EntityType mEntityType;
	sf::Sprite mSprite;

private:
	void setup();
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Entity() = default;
	Entity(Side side, EntityType entityType, const sf::Texture& texture);
	Entity(Side side, EntityType entityType, const sf::Texture& texture, sf::IntRect rect);
	virtual ~Entity();
public:
	sf::FloatRect getGlobalBounds() const;
	virtual void update(sf::Time dt) = 0;
};
#endif