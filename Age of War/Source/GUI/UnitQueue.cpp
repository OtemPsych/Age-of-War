#include "UnitQueue.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	UnitQueue::UnitData::UnitData(Unit::UnitType pType, sf::Time pSpawn)
		: type(pType)
		, spawn(pSpawn)
	{
	}

	UnitQueue::UnitQueue(sf::FloatRect spawnBarBounds)
	{
		setOrigin(-spawnBarBounds.width / 2.f, getOrigin().y);

		setupIdentifiers(spawnBarBounds);
	}

	void UnitQueue::update(const Identifier& id)
	{
		if (id == Identifier::Fill) {
			unsigned short idPos = mQueue.size() - 1;
			mIdentifiers[idPos].setFillColor(sf::Color(179, 89, 0));
		} 
		else if (!mQueue.empty()) {
			unsigned short idPos = mQueue.size();
			mIdentifiers[idPos].setFillColor(sf::Color(51, 51, 51));
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
													 -size.y * 2 - thickness));

			mIdentifiers[i].setFillColor(sf::Color(51, 51, 51));
			mIdentifiers[i].setOutlineThickness(thickness);
			mIdentifiers[i].setOutlineColor(sf::Color::White);
		}
	}

	void UnitQueue::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!mQueue.empty()) {
			states.transform *= getTransform();
			for (const auto& id : mIdentifiers)
				target.draw(id, states);
		}
	}

	void UnitQueue::push(const UnitData& data)
	{
		mQueue.push(std::move(data));
		update(Identifier::Fill);
	}

	UnitQueue::UnitData UnitQueue::getNextUnitData()
	{
		UnitData data(mQueue.front());
		mQueue.pop();
		update(Identifier::Empty);

		return std::move(data);
	}
}