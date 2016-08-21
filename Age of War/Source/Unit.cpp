#include "Unit.h"
#include "DataTables.h"

#include <PYRO/Math.h>

Unit::Unit(Side side, Unit* front_unit, data::UnitData* unit_data,
	       data::ValueDisplayData* value_display_data,
	       pyro::SoundPlayer<Unit::SoundID>* sound_player,
		   pyro::SceneNode* gui_scene_layer)
	: HealthEntity(side, unit_data)
	, damage_display_manager_(nullptr)
	, sound_player_(sound_player)
	, attack_animation_(nullptr)
	, enemy_targeted_(nullptr)
	, attack_rate_(sf::Time::Zero)
	, unit_state_(UnitState::Moving)
	, unit_data_(unit_data)
	, gui_scene_layer_(gui_scene_layer)
	, walking_animation_(nullptr)
	, front_unit_(front_unit)
{
	auto damage_display_manager(std::make_unique<ValueDisplayManager>(value_display_data));
	damage_display_manager_ = damage_display_manager.get();
	gui_scene_layer->attachChild(std::move(damage_display_manager));

	auto attack_animation(std::make_unique<Animation>(unit_data->attack_animation_data, this));
	attack_animation_ = attack_animation.get();
	attachChild(std::move(attack_animation));

	auto walking_animation(std::make_unique<Animation>(unit_data->walk_animation_data, this));
	walking_animation_ = walking_animation.get();
	attachChild(std::move(walking_animation));

	health_bar_->correctBarProperties(getLocalBounds());
}

Unit::~Unit()
{
	gui_scene_layer_->detachChild(*damage_display_manager_);
}

Unit::GeneralUnitType Unit::getGeneralUnitType() const
{
	return unit_data_->general_unit_type;
}

Unit::UnitType Unit::getUnitType() const
{
	return unit_data_->unit_type;
}

void Unit::attack(HealthEntity* enemy)
{
	if (enemy_targeted_) {
		if (attack_rate_ >= unit_data_->rate.value.current) {
			enemy_targeted_->receiveDamage(unit_data_->damage.value.current);
			damage_display_manager_->addValueDisplay(getGlobalBounds(), enemy_targeted_->getGlobalBounds(),
				                                     unit_data_->damage.value.current);

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

void Unit::nullifyEnemyTargeted()
{
	enemy_targeted_ = nullptr;
}

bool Unit::enemyInRange(HealthEntity* enemy)
{
	if (enemy) {
		const sf::FloatRect gbounds(getGlobalBounds());
		const sf::FloatRect enemy_gbounds(enemy->getGlobalBounds());

		if (side_ == Side::Ally) {
			if (gbounds.left + gbounds.width + unit_data_->range.value.current >= enemy_gbounds.left)
				return true;
		}
		else {
			if (gbounds.left - unit_data_->range.value.current <= enemy_gbounds.left + enemy_gbounds.width)
				return true;
		}
	}

	return false;
}

void Unit::updateCurrent(sf::Time dt)
{
	if (unit_state_ == UnitState::Moving) {
		walking_animation_->activateUpdating(ActivationTarget::Current, true);
		attack_animation_->activateUpdating(ActivationTarget::Current, false);
		if (side_ == Side::Ally)
			move(unit_data_->speed * dt.asSeconds(), 0.f);
		else
			move(-(unit_data_->speed * dt.asSeconds()), 0.f);
	}
	else if (unit_state_ == UnitState::Attacking) {
		walking_animation_->activateUpdating(ActivationTarget::Current, false);
		attack_animation_->activateUpdating(ActivationTarget::Current, true);
		attack_rate_ += dt;
	}
	else {
		walking_animation_->activateUpdating(ActivationTarget::Current, false);
		attack_animation_->activateUpdating(ActivationTarget::Current, false);
	}

	if (front_unit_ && getGlobalBounds().intersects(front_unit_->getGlobalBounds())) {
		if (unit_state_ == UnitState::Moving)
			unit_state_ = UnitState::Idle;
	}
	else {
		unit_state_ = enemy_targeted_ && enemyInRange(enemy_targeted_) ? UnitState::Attacking : UnitState::Moving;
	}

	//if (unit_state_ == UnitState::Moving && front_unit_ && getGlobalBounds().intersects(front_unit_->getGlobalBounds())) {
	//	unit_state_ = UnitState::Idle;
	//}

	//if (front_unit_ && getGlobalBounds().intersects(front_unit_->getGlobalBounds()))
	//	unit_state_ = UnitState::Idle;
	//else
	//	unit_state_ = enemy_targeted_ && enemyInRange(enemy_targeted_) ? UnitState::Attacking : UnitState::Moving;
}