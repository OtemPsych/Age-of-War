#include "Bar.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace gui
{
	Bar::Bar(sf::FloatRect entityBounds, bool horizontal, const sf::Color& color)
		: mOutline(sf::Vector2f(entityBounds.width, 0.055f * entityBounds.height))
		, mVertices(sf::Quads)
	{
		mVertices.append(sf::Vertex(sf::Vector2f(0.f,                0.f),                         color));
		mVertices.append(sf::Vertex(sf::Vector2f(entityBounds.width, 0.f),                         color));
		mVertices.append(sf::Vertex(sf::Vector2f(entityBounds.width, 0.055f * entityBounds.height), color));
		mVertices.append(sf::Vertex(sf::Vector2f(0.f,                0.055f * entityBounds.height), color));

		setOrigin(entityBounds.width / 2.f, getOrigin().y);

		mOutline.setFillColor(sf::Color(51, 51, 51));
		mOutline.setOutlineColor(sf::Color::White);
		mOutline.setOutlineThickness(0.015f * entityBounds.width);

		if (horizontal) {
			setPosition(0.f, -entityBounds.height / 2.f - mVertices[2].position.y - mOutline.getOutlineThickness() - 10.f);
		}
		else {
			rotate(-90.f);
			setPosition(-entityBounds.width / 2.f - mVertices[2].position.y - mOutline.getOutlineThickness() - 2.f, 0.f);
		}
	}

	Bar::~Bar()
	{
	}

	sf::FloatRect Bar::getGlobalBounds() const
	{
		return mOutline.getGlobalBounds();
	}

	void Bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mOutline, states.transform *= getTransform());
		target.draw(mVertices, states);
	}

	void Bar::update(unsigned total, unsigned current)
	{
		mVertices[1].position.x =
		mVertices[2].position.x = current * mOutline.getSize().x / total;
	}
}