#ifndef Unit_H_
#define Unit_H_

#include "HealthEntity.h"
#include "Animation.h"
#include "ValueDisplayManager.h"

#include <PYRO/Audio/SoundPlayer.h>

namespace data { struct UnitData; struct ValueDisplayData; }
class Unit : public HealthEntity
{
public:
	enum class GeneralUnitType { Melee, Ranged };
	enum UnitType { Mage, Knight, Samurai, Shadow, Destroyer, Executioner, UnitCount };
	enum class SoundID { MageAttack, KnightAttack, SoundCount };
protected:
	enum class UnitState { Idle, Moving, Attacking, StateCount };

public:
	Unit(Side side, Unit* front_unit, data::UnitData* unit_data,
		 data::ValueDisplayData* value_display_data,
		 pyro::SoundPlayer<Unit::SoundID>* sound_player,
		 pyro::SceneNode* gui_scene_layer);
	virtual ~Unit();
public:
	GeneralUnitType getGeneralUnitType() const;
	UnitType        getUnitType() const;

	virtual void attack(HealthEntity* enemy);
	virtual void nullifyEnemyTargeted();
	inline void nullifyFrontUnit() { front_unit_ = nullptr; }
protected:
	bool enemyInRange(HealthEntity* enemy);
	virtual void updateCurrent(sf::Time dt) override;

protected:
	ValueDisplayManager*        damage_display_manager_;
	pyro::SoundPlayer<SoundID>* sound_player_;
	Animation*                  attack_animation_;

	HealthEntity*               enemy_targeted_;
	sf::Time                    attack_rate_;
	UnitState                   unit_state_;
private:
	data::UnitData*             unit_data_;
	pyro::SceneNode*            gui_scene_layer_;
	Animation*                  walking_animation_;

	Unit*                       front_unit_;
};
#endif