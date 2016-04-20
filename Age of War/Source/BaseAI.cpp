#include "BaseAI.h"

BaseAI::BaseAI(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
			   const pyro::TextureHolder<Unit::Type>& textures,
			   std::vector<Unit::UnitData>& data,
			   pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(side, worldBounds, baseTexture, textures, data, soundPlayer)
{
}

void BaseAI::update(sf::Time dt)
{
	handleUnitSpawn(Unit::Type::Knight);
	Base::update(dt);
}