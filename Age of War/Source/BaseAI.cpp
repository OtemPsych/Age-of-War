#include "BaseAI.h"

BaseAI::BaseAI(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
			   const pyro::TextureHolder<Unit::UnitType>& textures,
			   std::vector<gStruct::UnitData>& data,
			   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(side, worldBounds, baseTexture, textures, data, soundPlayer)
{
}

void BaseAI::update(sf::Time dt)
{
	handleUnitSpawn(static_cast<Unit::UnitType>(rand() % Unit::UnitType::TypeCount));
	Base::update(dt);
}