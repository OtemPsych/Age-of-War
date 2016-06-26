#ifndef Turret_H_
#define Turret_H_

#include "Unit.h"
#include "GlobalStructs.h"

class Turret : public Entity
{
public:
	enum TurretType { LaserTurret, TypeCount };
private:
	TurretType                   mTurretType;
	std::vector<sf::VertexArray> mProjectiles;
	unsigned short               mDamage;
	float                        mRange;
	gStruct::Resource<sf::Time>  mAttackRate;
	float                        mProjectileSpeed;

	sf::Vector2f                 mEnemyPosition;

	ValueDisplays                mDamageDisplays;

private:
	void spawnProjectile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Turret(Side side, sf::Font& damageDisplayFont, sf::Vector2f baseSize, gStruct::TurretData& data,
		   const pyro::TextureHolder<TurretType>& textures);
public:
	void drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const;
	void attack(Unit& unit);
	virtual void update(sf::Time dt) override;
};
#endif