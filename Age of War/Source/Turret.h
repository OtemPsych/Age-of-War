#ifndef Turret_H_
#define Turret_H_

#include "Unit.h"
#include "GlobalStructs.h"

class Turret : public AttackEntity
{
public:
	enum TurretType { LaserTurret, TypeCount };
private:
	TurretType                   mTurretType;
	std::vector<sf::VertexArray> mProjectiles;
	float                        mProjectileSpeed;

	sf::Vector2f                 mEnemyPosition;

private:
	void spawnProjectile();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Turret(Side side, sf::Vector2f baseSize, sf::Font& font, gStruct::TurretData& data,
		   const pyro::TextureHolder<TurretType>& textures);
public:
	virtual void attack(HealthEntity& enemy) override;
	virtual void update(sf::Time dt) override;
};
#endif