#include "Bar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	Bar::Bar(sf::FloatRect entityBounds, bool horizontal, const sf::Color& color)
		: mShape(sf::Vector2f(entityBounds.width, 0.055f * entityBounds.height))
		, mOutline(mShape.getSize())
	{
		centerOrigin();

		mShape.setFillColor(color);
		mOutline.setFillColor(sf::Color(51, 51, 51));
		mOutline.setOutlineColor(sf::Color::White);
		mOutline.setOutlineThickness(0.015f * entityBounds.width);

		if (horizontal) 
			setPosition(0.f, -entityBounds.height / 2.f - mOutline.getOutlineThickness() - 10.f);
		else
		{
			rotate(-90.f);
			setPosition(-(entityBounds.width / 2.f) - mOutline.getOutlineThickness() - 8.f, 0.f);
		}
	}

	Bar::~Bar()
	{
	}

	void Bar::centerOrigin()
	{
		sf::FloatRect shapeLBounds(mShape.getLocalBounds());
		mShape.setOrigin(shapeLBounds.width / 2.f, shapeLBounds.height / 2.f);

		sf::FloatRect outlineLBounds(mOutline.getLocalBounds());
		mOutline.setOrigin(outlineLBounds.width / 2.f, outlineLBounds.height / 2.f);
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