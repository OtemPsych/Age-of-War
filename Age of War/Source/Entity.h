#ifndef Entity_H_
#define Entity_H_

#include "GUI/Bar.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Entity : public sf::Transformable, public sf::Drawable
{
public:
	enum class Side { Left, Right };
protected:
	enum class Type { Base, Unit };
private:
	struct Health
	{
		const unsigned original;
		unsigned	   current;

		explicit Health(unsigned health);
	};
protected:
	sf::Sprite mSprite;
	Side	   mSide;
private:
	Health	   mHealth;
	gui::Bar   mHealthBar;

	bool	   mIsDestroyable;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Entity(Side side, Type type, unsigned health,
		   const sf::Texture& texture, sf::IntRect textureRect = sf::IntRect());
	virtual ~Entity();
public:
	void reduceHealth(unsigned damage);
	virtual void update(sf::Time dt);

	sf::FloatRect getGlobalBounds() const;
	inline bool isDestroyable() const { return mIsDestroyable; }
};
#endif