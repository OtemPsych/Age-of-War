#ifndef Turret_H_
#define Turret_H_

#include <PYRO/VertexArrayNode.h>

#include "Unit.h"

namespace data { struct TurretData; }
class Turret : public Entity
{
public:
	enum TurretType { LaserTurret, TurretCount };

public:
	Turret(const sf::Font& damage_font, const sf::Vector2f& base_size, Side side,
		   data::TurretData* turret_data, data::ValueDisplayData* value_display_data,
		   pyro::SceneNode* gui_scene_layer, pyro::SceneNode* proj_scene_layer);
	~Turret();
public:
	void attack(HealthEntity* enemy);
	void nullifyEnemyTargeted();
private:
	void spawnProjectile();
	bool enemyInRange(HealthEntity* enemy);
	virtual void updateCurrent(sf::Time dt) override;

private:
	data::TurretData*                   turret_data_;
	pyro::SceneNode*                    gui_scene_layer_;
	pyro::SceneNode*                    proj_scene_layer_;
	HealthEntity*                       enemy_targeted_;

	std::vector<pyro::VertexArrayNode*> projectiles_;
	sf::Vector2f                        enemy_position_;
	sf::Time                            attack_rate_;
	ValueDisplayManager*                damage_display_manager_;
};
#endif