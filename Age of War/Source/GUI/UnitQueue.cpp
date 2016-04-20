#include "UnitQueue.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	UnitQueue::UnitData::UnitData(Unit::Type pType, sf::Time pSpawn)
		: type(pType)
		, spawn(pSpawn)
	{
	}

	UnitQueue::UnitQueue(sf::FloatRect spawnBarBounds)
	{
		setupIdentifiers(spawnBarBounds);
	}

	void UnitQueue::update()
	{
		for (unsigned i = 0; i < mIdentifiers.size(); i++)
		{
			sf::Color newColor = i < mQueue.size() ? sf::Color(179, 89, 0) : sf::Color(51, 51, 51);
			mIdentifiers[i].setFillColor(newColor);
		}
	}

	void UnitQueue::setupIdentifiers(sf::FloatRect spawnBarBounds)
	{
		const sf::Vector2f size(10.f, 10.f);
		const float thickness = 1.f;
		const float margin = 8.f;

		for (size_t i = 0; i < mIdentifiers.size(); i++)
		{
			mIdentifiers[i].setSize(size);

			mIdentifiers[i].setPosition(sf::Vector2f((size.x + margin) * i - margin * mIdentifiers.size(),
													 -margin - size.y));

			mIdentifiers[i].setOutlineThickness(thickness);
			mIdentifiers[i].setOutlineColor(sf::Color::White);
		}
	}

	void UnitQueue::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!mQueue.empty())
		{
			states.transform *= getTransform();
			for (const auto& id : mIdentifiers)
				target.draw(id, states);
		}
	}

	void UnitQueue::push(const UnitData& data)
	{
		mQueue.push(data);
		update();
	}

	UnitQueue::UnitData UnitQueue::getNextUnitData()
	{
		UnitData data(mQueue.front());
		mQueue.pop();
		update();

		return data;
	}
}