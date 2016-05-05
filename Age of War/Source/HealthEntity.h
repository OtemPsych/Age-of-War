#ifndef HealthEntity_H_
#define HealthEntity_H_

#include "Entity.h"
#include "GlobalStructs.h"
#include "GUI/Bar.h"

class HealthEntity : public Entity
{
private:
	gStruct::Resource<unsigned short> mHealth;
	gui::Bar                          mHealthBar;
	bool                              mIsDestroyable;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	HealthEntity(Side side, EntityType entityType, unsigned short health, const sf::Texture& texture);
	HealthEntity(Side side, EntityType entityType, unsigned short health, const sf::Texture& texture, sf::IntRect rect);
	virtual ~HealthEntity();
public:
	void receiveDamage(unsigned short damage);
	virtual void update(sf::Time dt) = 0;

	inline bool isDestroyable() const { return mIsDestroyable; }
};
#endif