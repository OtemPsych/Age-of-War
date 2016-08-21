#include "Bar.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace gui
{
	Bar::Bar(const sf::FloatRect& entity_lbounds, const sf::Color& color, bool horizontal)
		: VertexArrayNode(sf::Vector2f(0.f, 0.f))
		, color_(color)
		, horizontal_(horizontal)
	{
		setOriginFlags(pyro::utils::OriginFlag::Center);
		correctBarProperties(entity_lbounds);
	}

	Bar::~Bar()
	{
	}

	void Bar::correctBarProperties(const sf::FloatRect& entity_lbounds)
	{
		vertices_.clear();

		const float padding = entity_lbounds.width * 0.015f;
		const sf::Vector2f padded_edges(entity_lbounds.width - padding, 0.068f * entity_lbounds.height);
		const sf::Color background_color(sf::Color(51, 51, 51));

		// Outline
		vertices_.append(sf::Vertex(sf::Vector2f(0.f,                  0.f)));
		vertices_.append(sf::Vertex(sf::Vector2f(entity_lbounds.width, 0.f)));
		vertices_.append(sf::Vertex(sf::Vector2f(entity_lbounds.width, padded_edges.y)));
		vertices_.append(sf::Vertex(sf::Vector2f(0.f,                  padded_edges.y)));
		// Background
		vertices_.append(sf::Vertex(sf::Vector2f(padding,        padding),                  background_color));
		vertices_.append(sf::Vertex(sf::Vector2f(padded_edges.x, padding),                  background_color));
		vertices_.append(sf::Vertex(sf::Vector2f(padded_edges.x, padded_edges.y - padding), background_color));
		vertices_.append(sf::Vertex(sf::Vector2f(padding,        padded_edges.y - padding), background_color));
		// Actual
		vertices_.append(sf::Vertex(sf::Vector2f(padding,        padding),                  color_));
		vertices_.append(sf::Vertex(sf::Vector2f(padded_edges.x, padding),                  color_));
		vertices_.append(sf::Vertex(sf::Vector2f(padded_edges.x, padded_edges.y - padding), color_));
		vertices_.append(sf::Vertex(sf::Vector2f(padding,        padded_edges.y - padding), color_));

		correntOriginFlags();

		if (horizontal_) {
			setPosition(entity_lbounds.width / 2.f, -padded_edges.y);
		}
		else {
			rotate(-90.f);
			setPosition(-padded_edges.y / 2.f - 1.f, entity_lbounds.height / 2.f);
		}
	}
}