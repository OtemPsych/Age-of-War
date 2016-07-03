#include "BaseAI.h"

BaseAI::BaseAI(Side side, sf::IntRect worldBounds, sf::Font& font, const sf::Texture& baseTexture,
	           const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	           const pyro::TextureHolder<Turret::TurretType>& turretTextures,
	           pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(side, worldBounds, font, baseTexture, unitTextures, turretTextures, soundPlayer)
{
}

void BaseAI::update(sf::Time dt)
{
	handleTurretSpawn(static_cast<Turret::TurretType>(rand() % Turret::TurretType::TypeCount), rand() % 3);
	handleUnitSpawn(static_cast<Unit::UnitType>(rand() % Unit::UnitType::TypeCount));

	Base::update(dt);
}