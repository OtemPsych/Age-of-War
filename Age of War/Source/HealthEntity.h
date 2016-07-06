#ifndef HealthEntity_H_
#define HealthEntity_H_

#include <PYRO/Text.h>

#include "Entity.h"
#include "GUI/Bar.h"

class HealthEntity : public Entity
{
protected:
	pyro::utils::Resource<float> mHealth;
private:
	gui::Bar                     mHealthBar;
	bool                         mIsDestroyable;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	HealthEntity(Side side, EntityType entityType, float health, const sf::Texture& texture);
	HealthEntity(Side side, EntityType entityType, float health, const sf::Texture& texture, sf::IntRect rect);
	virtual ~HealthEntity();
public:
	void receiveDamage(unsigned short damage);
	virtual void update(sf::Time dt) = 0;

	inline bool isDestroyable() const { return mIsDestroyable; }
	inline sf::Vector2f getSpriteOrigin() const { return mSprite.getOrigin(); }
};
#endif