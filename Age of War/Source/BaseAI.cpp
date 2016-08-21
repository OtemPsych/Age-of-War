#include "BaseAI.h"

BaseAI::BaseAI(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
	           const pyro::TextureHolder<Unit::UnitType>& unit_textures,
	           const pyro::TextureHolder<Turret::TurretType>& turret_textures,
	           Side side, data::BaseData* base_data,
	           pyro::SoundPlayer<Unit::SoundID>* sound_player,
	           const std::vector<pyro::SceneNode*>* scene_layers)
	: Base(world_bounds, damage_font, unit_textures, turret_textures, side, base_data, sound_player, scene_layers)
{
}
void BaseAI::updateCurrent(sf::Time dt)
{
	handleTurretSpawn(static_cast<Turret::TurretType>(rand() % Turret::TurretType::TurretCount), rand() % 3);
	handleUnitSpawn(static_cast<Unit::UnitType>(rand() % Unit::UnitType::UnitCount));

	Base::updateCurrent(dt);
}