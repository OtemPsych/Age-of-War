#ifndef Turret_H_
#define Turret_H_

#include "Unit.h"
#include "GlobalStructs.h"

class Turret : public sf::Transformable, public sf::Drawable
{
public:
	enum class Type { Turret1 };
private:
	sf::Sprite                   mSprite;
	std::vector<sf::VertexArray> mProjectiles;
	unsigned                     mDamage;
	float                        mRange;
	gStruct::Resource<sf::Time>  mAttackRate;

private:
	void spawnProjectile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Turret(Type type, sf::Vector2f baseSize, const sf::Texture& texture, unsigned damage,
		   float range, sf::Time attackRate);
public:
	void attack(Unit& unit);
	void update(sf::Time dt);

	sf::FloatRect getGlobalBounds() const;
};
#endif