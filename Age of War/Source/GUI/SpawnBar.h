#ifndef SpawnBar_H_
#define SpawnBar_H_

#include "Bar.h"
#include "UnitQueue.h"

#include "../Unit.h"

namespace gui
{
	class SpawnBar : public Bar
	{
	private:
		bool	   mSpawning;
		Unit::Type mUnitType;
		sf::Time   mUnitSpawnTime;
		sf::Time   mElapsedTime;

		UnitQueue  mUnitQueue;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	public:
		SpawnBar(sf::FloatRect unitBounds, bool horizontal, const sf::Color& color);
	public:
		bool update(sf::Time dt);
		bool spawnNewUnit(Unit::Type type, sf::Time spawnTime);

		inline Unit::Type getUnitTypeSpawning() const { return mUnitType; }
	};
}
#endif