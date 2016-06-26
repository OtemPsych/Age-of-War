#ifndef AttackEntity_H_
#define AttackEntity_H_

#include "HealthEntity.h"
#include "Animation.h"

#include <PYRO/Text.h>

class AttackEntity : public virtual Entity
{
protected:
	struct DamageDisplay {
		pyro::Text                      text;
		sf::Vector2f                    velocity;
		sf::Vector2f                    acceleration;
		pyro::utils::Resource<sf::Time> lifetime;

		DamageDisplay(sf::FloatRect enemyGBounds, sf::Font& font, unsigned short damage, bool ally);
		void operator=(const DamageDisplay& copy);
	};
private:
	sf::Font&                       mDamageDisplayFont;
protected:
	const float                     mAttackRange;
	pyro::utils::Resource<sf::Time> mAttackRate;
	const unsigned short            mDamage;
	bool                            mAttacking;

	std::vector<DamageDisplay>      mDamageDisplays;

protected:
	void addDamageDisplay(const sf::FloatRect& enemyGBounds);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
public:
	AttackEntity(Side side, EntityType entityType, float attackRange, const sf::Time& attackRate,
		         unsigned short damage, sf::Font& damageDisplayFont, const sf::Texture& texture);
	AttackEntity(Side side, EntityType entityType, float attackRange, const sf::Time& attackRate,
		         unsigned short damage, sf::Font& damageDisplayFont, const sf::Texture& texture, sf::IntRect rect);
	virtual ~AttackEntity();
public:
	void drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void attack(HealthEntity& enemy) = 0;
	virtual void update(sf::Time dt) override = 0;

	inline void stopAttacking() { mAttacking = false; }
};
#endif