#ifndef RangedUnit_H_
#define RangedUnit_H_

#include <PYRO/VertexArrayNode.h>

#include "Unit.h"

namespace data { struct RangedUnitData; }
class RangedUnit : public Unit
{
public:
	enum class ProjectileTexture { Destroyer, ProjectileTextureCount };

public:
	RangedUnit(Side side, Unit* front_unit,
		       data::RangedUnitData* ranged_unit_data,
		       data::ValueDisplayData* value_display_data,
		       pyro::SoundPlayer<Unit::SoundID>* sound_player,
			   pyro::SceneNode* gui_scene_layer, pyro::SceneNode* proj_scene_layer);
	~RangedUnit();
public:
	virtual void attack(HealthEntity* enemy) override;
	virtual void nullifyEnemyTargeted() override;
private:
	virtual void updateCurrent(sf::Time dt) override;

private:
	data::RangedUnitData*               ranged_unit_data_;
	pyro::SceneNode*                    proj_scene_layer_;
	std::vector<pyro::VertexArrayNode*> projectiles_;
};
#endif