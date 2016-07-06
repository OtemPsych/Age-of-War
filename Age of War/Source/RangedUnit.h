#ifndef RangedUnit_H_
#define RangedUnit_H_

#include "Unit.h"

#include <SFML/Graphics/VertexArray.hpp>

#include <functional>

class RangedUnit : public Unit
{
public:
	enum class ProjectileTexture { Destroyer };
private:
	std::vector<sf::VertexArray>                            mProjectiles;
	float                                                   mProjectileSpeed;
	data::UnitData::RangedUnitData::SpawnProjectileFunction mSpawnProjectile;

	sf::Vector2f                                            mEnemyPosition;

private:
	virtual void handleAttackAnimation(HealthEntity& enemy) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	RangedUnit(Side side, sf::Font& font, data::UnitData& data,
		       const pyro::TextureHolder<UnitType>& textures, pyro::SoundPlayer<SoundID>& soundPlayer);
public:
	virtual void attack(HealthEntity& enemy) override;
	virtual void update(sf::Time dt) override;
};
#endif