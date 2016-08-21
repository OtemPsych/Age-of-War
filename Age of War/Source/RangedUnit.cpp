#include "RangedUnit.h"
#include "DataTables.h"

#include <PYRO/Math.h>

RangedUnit::RangedUnit(Side side, Unit* front_unit,
	                   data::RangedUnitData* ranged_unit_data,
	                   data::ValueDisplayData* value_display_data,
	                   pyro::SoundPlayer<Unit::SoundID>* sound_player,
					   pyro::SceneNode* gui_scene_layer, pyro::SceneNode* proj_scene_layer)
	: Unit(side, front_unit, ranged_unit_data, value_display_data, sound_player, gui_scene_layer)
	, ranged_unit_data_(ranged_unit_data)
	, proj_scene_layer_(proj_scene_layer)
{
}

RangedUnit::~RangedUnit()
{
	for (auto& itr = projectiles_.begin(); itr != projectiles_.end(); itr++) {
		proj_scene_layer_->detachChild(*(*itr));
	}
	projectiles_.clear();
}

void RangedUnit::attack(HealthEntity* enemy)
{
	if (enemy_targeted_) {
		if (attack_rate_ >= ranged_unit_data_->rate.value.current) {
			ranged_unit_data_->spawn_projectile(&projectiles_, getGlobalBounds(), proj_scene_layer_);
			if (side_ == Side::Enemy)
				projectiles_.back()->move(-getGlobalBounds().width, 0.f);

			attack_rate_ = sf::Time::Zero;
			attack_animation_->restart();
		}
	}
	else if (enemyInRange(enemy)) {
		enemy_targeted_ = enemy;
		unit_state_ = UnitState::Attacking;

		attack_rate_ = sf::Time::Zero;
		attack_animation_->restart();
	}
}

void RangedUnit::nullifyEnemyTargeted()
{
	enemy_targeted_ = nullptr;

	for (auto& itr = projectiles_.begin(); itr != projectiles_.end(); itr++) {
		proj_scene_layer_->detachChild(*(*itr));
	}
	projectiles_.clear();
}

void RangedUnit::updateCurrent(sf::Time dt)
{
	if (enemy_targeted_) {
		for (auto& projectile : projectiles_) {
			const sf::Vector2f proj_pos(projectile->getPosition());
			const sf::Vector2f enemy_pos(enemy_targeted_->getPosition());
			const float distance = pyro::math::getHypotenuse(enemy_pos - proj_pos);

			sf::Vector2f velocity(ranged_unit_data_->projectile_speed * (enemy_pos.x - proj_pos.x) / distance,
								  ranged_unit_data_->projectile_speed * (enemy_pos.y - proj_pos.y) / distance);

			projectile->move(velocity * dt.asSeconds());
		}

		for (size_t i = 0; i < projectiles_.size(); ++i) {
			if (enemy_targeted_->getGlobalBounds().intersects(projectiles_[i]->getGlobalBounds())) {
				enemy_targeted_->receiveDamage(ranged_unit_data_->damage.value.current);
				damage_display_manager_->addValueDisplay(getGlobalBounds(), enemy_targeted_->getGlobalBounds(),
					ranged_unit_data_->damage.value.current);

				proj_scene_layer_->detachChild(*projectiles_[i]);
				projectiles_.erase(projectiles_.begin() + i);
			}
		}
	}

	Unit::updateCurrent(dt);
}