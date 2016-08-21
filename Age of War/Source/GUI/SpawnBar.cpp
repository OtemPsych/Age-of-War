#include "SpawnBar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	SpawnBar::SpawnBar(const sf::FloatRect& unit_lbounds, bool horizontal)
		: Bar(unit_lbounds, sf::Color(153, 77, 0), horizontal)
		, spawning_(false)
		, spawn_time_(sf::Time::Zero)
		, next_spawn_ready_(false)
		, unit_queue_(nullptr)
	{
		auto unit_queue(std::make_unique<UnitQueue>(getLocalBounds()));
		unit_queue_ = unit_queue.get();
		attachChild(std::move(unit_queue));

		vertices_[9].color =
		vertices_[10].color = sf::Color(179, 89, 0);
	}

	bool SpawnBar::spawnNewUnit(const UnitQueue::UnitData& data)
	{
		if (!spawning_) {
			unit_type_ = data.type;
			spawn_time_.original = data.spawn_time;
			spawn_time_.current = sf::Time::Zero;
			spawning_ = true;

			return true;
		}
		else if (!unit_queue_->full()) {
			unit_queue_->push(std::move(data));
			return true;
		}

		return false;
	}

	void SpawnBar::updateCurrent(sf::Time dt)
	{
		activateDrawing(ActivationTarget::Both, spawning_);

		if (spawning_) {
			if ((spawn_time_.current += dt) >= spawn_time_.original) {
				spawn_time_.current = sf::Time::Zero;
				spawning_ = false;

				next_spawn_ready_ = true;
				return;
			}
			else {
				vertices_[9].position.x =
				vertices_[10].position.x = spawn_time_.current * (vertices_[5].position.x - vertices_[4].position.x)
					                      / spawn_time_.original;
			}
		}
		else if (!unit_queue_->empty()) {
			spawnNewUnit(std::move(unit_queue_->getNextUnitData()));
		}

		next_spawn_ready_ = false;
	}
}