#ifndef BaseAI_H_
#define BaseAI_H_

#include "Base.h"

class BaseAI : public Base
{
public:
	BaseAI(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		   const pyro::TextureHolder<Unit::UnitType>& textures,
		   std::vector<gStruct::UnitData>& data,
		   pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	virtual void update(sf::Time dt) override;
};
#endif