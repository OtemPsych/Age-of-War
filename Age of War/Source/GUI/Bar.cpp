#include "Bar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	Bar::Bar(sf::FloatRect entityBounds, bool horizontal, const sf::Color& color)
		: mShape(sf::Vector2f(entityBounds.width, 5.f * entityBounds.height / 100.f))
		, mOutline(mShape.getSize())
	{
		mShape.setFillColor(color);
		mOutline.setFillColor(sf::Color(51, 51, 51));
		mOutline.setOutlineColor(sf::Color::White);
		mOutline.setOutlineThickness(0.015f * entityBounds.width);

		if (horizontal)
			setPosition(0.f, -mOutline.getOutlineThickness() - 20.f);
		else
		{
			rotate(-90.f);
			setPosition(0.f - mShape.getSize().y - mOutline.getOutlineThickness() - 2.f,
						entityBounds.height - mOutline.getOutlineThickness() - 20.f);
		}
	}

	Bar::~Bar()
	{
	}

	sf::FloatRect Bar::getGlobalBounds() const
	{
		return mShape.getGlobalBounds();
	}

	void Bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(mOutline, states);
		target.draw(mShape, states);
	}

	void Bar::update(unsigned total, unsigned current)
	{
		sf::Vector2f outlineSize(mOutline.getSize());
		mShape.setSize(sf::Vector2f(current * outlineSize.x / total, outlineSize.y));
	}
}