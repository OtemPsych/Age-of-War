#include "SpawnBar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	SpawnBar::SpawnBar(sf::FloatRect unitBounds, bool horizontal, const sf::Color& color)
		: Bar(unitBounds, horizontal, color)
		, mSpawning(false)
		, mUnitSpawnTime(sf::Time::Zero)
		, mElapsedTime(sf::Time::Zero)
		, mUnitQueue(getGlobalBounds())
	{
	}

	void SpawnBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mSpawning)
			Bar::draw(target, states);

		target.draw(mUnitQueue, states.transform *= getTransform());
	}

	bool SpawnBar::update(sf::Time dt)
	{
		if (mSpawning)
			if ((mElapsedTime += dt) >= mUnitSpawnTime)
			{
				mElapsedTime = sf::Time::Zero;
				mSpawning = false;
				return true;
			}
			else
				Bar::update(mUnitSpawnTime.asMilliseconds(), mElapsedTime.asMilliseconds());
		else if (!mUnitQueue.empty())
		{
			UnitQueue::UnitData data(mUnitQueue.getNextUnitData());
			spawnNewUnit(data.type, data.spawn);
		}

		return false;
	}

	bool SpawnBar::spawnNewUnit(Unit::Type type, sf::Time spawnTime)
	{
		if (!mSpawning)
		{
			mUnitType = type;
			mUnitSpawnTime = spawnTime;
			mSpawning = true;

			return true;
		}
		else if (!mUnitQueue.full())
		{
			mUnitQueue.push(UnitQueue::UnitData(type, spawnTime));
			return true;
		}

		return false;
	}
}