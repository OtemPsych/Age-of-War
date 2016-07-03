#ifndef BaseAI_H_
#define BaseAI_H_

#include "Base.h"

class BaseAI : public Base
{
public:
	BaseAI(Side side, sf::IntRect worldBounds, sf::Font& font, const sf::Texture& baseTexture,
		   const pyro::TextureHolder<Unit::UnitType>& unitTextures,
		   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		   pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	virtual void update(sf::Time dt) override;
};
#endif