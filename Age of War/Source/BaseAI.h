#ifndef BaseAI_H_
#define BaseAI_H_

#include "Base.h"

class BaseAI : public Base
{
public:
	BaseAI(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		   const pyro::TextureHolder<Unit::UnitType>& unitTextures,
		   std::vector<gStruct::UnitData>& unitData,
		   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		   std::vector<gStruct::TurretData>& turretData,
		   pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	virtual void update(sf::Time dt) override;
};
#endif