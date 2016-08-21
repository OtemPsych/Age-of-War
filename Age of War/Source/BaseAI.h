#ifndef BaseAI_H_
#define BaseAI_H_

#include "Base.h"

class BaseAI : public Base
{
public:
	BaseAI(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
		   const pyro::TextureHolder<Unit::UnitType>& unit_textures,
		   const pyro::TextureHolder<Turret::TurretType>& turret_textures,
		   Side side, data::BaseData* base_data,
		   pyro::SoundPlayer<Unit::SoundID>* sound_player,
		   const std::vector<pyro::SceneNode*>* scene_layers);
public:
	virtual void updateCurrent(sf::Time dt) override;
};
#endif