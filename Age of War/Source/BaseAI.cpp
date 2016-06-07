#include "BaseAI.h"

BaseAI::BaseAI(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
	           const pyro::TextureHolder<Unit::UnitType>& unitTextures,
	           std::vector<gStruct::UnitData>& unitData,
	           const pyro::TextureHolder<Turret::TurretType>& turretTextures,
	           std::vector<gStruct::TurretData>& turretData,
	           pyro::SoundPlayer<Unit::SoundID>& soundPlayer)
	: Base(side, worldBounds, baseTexture, unitTextures, unitData, turretTextures, turretData, soundPlayer)
{
}

void BaseAI::update(sf::Time dt)
{
	handleTurretSpawn(Turret::TurretType::LaserTurret, 0);
	//handleUnitSpawn(static_cast<Unit::UnitType>(rand() % Unit::UnitType::TypeCount));
	Base::update(dt);
}