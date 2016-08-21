#ifndef UnitQueue_H_
#define UnitQueue_H_

#include "../Unit.h"

#include <queue>
#include <array>

namespace gui
{
	class UnitQueue : public pyro::SceneNode, private sf::NonCopyable
	{
	private:
		enum class Identifier { Fill, Empty };
	public:
		struct UnitData {
			UnitData(Unit::UnitType type, sf::Time spawn_time);

			Unit::UnitType type;
			sf::Time       spawn_time;
		};

	public:
		explicit UnitQueue(const sf::FloatRect& spawn_bar_bounds);
	public:
		void push(const UnitData& unit_data);
		UnitData getNextUnitData();

		virtual sf::FloatRect getLocalBounds() const override;

		inline bool empty() const { return queue_.size() == 0; }
		inline bool full() const { return queue_.size() == identifiers_.size(); }
	private:
		void updateIdentifiers(Identifier id);
		void setupIdentifiers(const sf::FloatRect& spawn_bar_bounds);
		virtual void updateCurrent(sf::Time dt) override;

	private:
		std::queue<UnitData>                  queue_;
		std::array<pyro::VertexArrayNode*, 5> identifiers_;
	};
}
#endif