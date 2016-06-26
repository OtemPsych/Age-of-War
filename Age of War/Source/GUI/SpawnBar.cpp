#include "SpawnBar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	SpawnBar::SpawnBar(sf::FloatRect unitBounds, bool horizontal)
		: Bar(unitBounds, horizontal, sf::Color(153, 77, 0))
		, mSpawning(false)
		, mUnitSpawnTime(sf::Time::Zero)
		, mElapsedTime(sf::Time::Zero)
		, mUnitQueue(getGlobalBounds())
	{
		mVertices[1].color =
		mVertices[2].color = sf::Color(179, 89, 0);
	}

	void SpawnBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mSpawning) {
			Bar::draw(target, states);
			target.draw(mUnitQueue, states.transform *= getTransform());
		}
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
			spawnNewUnit(std::move(mUnitQueue.getNextUnitData()));

		return false;
	}

	bool SpawnBar::spawnNewUnit(const UnitQueue::UnitData& data)
	{
		if (!mSpawning)
		{
			mUnitType = data.type;
			mUnitSpawnTime = data.spawn;
			mSpawning = true;

			return true;
		}
		else if (!mUnitQueue.full())
		{
			mUnitQueue.push(std::move(data));
			return true;
		}

		return false;
	}
}