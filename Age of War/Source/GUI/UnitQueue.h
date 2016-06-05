#ifndef UnitQueue_H_
#define UnitQueue_H_

#include "../Unit.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <queue>
#include <array>

namespace gui
{
	class UnitQueue : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		enum class Identifier { Fill, Empty };
	public:
		struct UnitData {
			Unit::UnitType type;
			sf::Time       spawn;

			UnitData(Unit::UnitType pType, sf::Time pSpawn);
		};
	private:
		std::queue<UnitData>			  mQueue;
		std::array<sf::RectangleShape, 5> mIdentifiers;

	private:
		void update(const Identifier& id);
		void setupIdentifiers(sf::FloatRect spawnBarBounds);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		explicit UnitQueue(sf::FloatRect spawnBarBounds);
	public:
		void push(const UnitData& data);
		UnitData getNextUnitData();

		inline bool empty() const { return mQueue.size() == 0; }
		inline bool full() const { return mQueue.size() == mIdentifiers.size(); }
	};
}
#endif