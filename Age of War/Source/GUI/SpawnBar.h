#ifndef SpawnBar_H_
#define SpawnBar_H_

#include "Bar.h"
#include "UnitQueue.h"

#include "../Unit.h"

namespace gui
{
	class SpawnBar : public Bar
	{
	public:
		SpawnBar(const sf::FloatRect& unit_lbounds, bool horizontal);
	public:
		bool spawnNewUnit(const UnitQueue::UnitData& data);

		inline Unit::UnitType getUnitTypeSpawning() const { return unit_type_; }
		inline bool isNextSpawnReady() const { return next_spawn_ready_; }
	private:
		virtual void updateCurrent(sf::Time dt) override;

	private:
		bool	                        spawning_;
		Unit::UnitType                  unit_type_;
		pyro::utils::Resource<sf::Time> spawn_time_;
					                    
		bool                            next_spawn_ready_;
					                    
		UnitQueue*                      unit_queue_;
	};
}
#endif