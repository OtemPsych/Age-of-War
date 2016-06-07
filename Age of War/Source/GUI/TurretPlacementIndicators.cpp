#include "TurretPlacementIndicators.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

namespace gui
{
	TurretPlacementIndicators::TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& windowRects,
		                                                 sf::RenderWindow& window)
		: mIndicators(sf::Quads)
		, mHoverLayer(nullptr)
		, mSelectedIndicator(-1)
		, mShouldCreateTurret(false)
		, mWindow(window)
	{
		setup(windowRects);
	}

	bool TurretPlacementIndicators::hover(unsigned indicator)
	{
		const unsigned startVert = indicator * 32;
		const sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow));

		return mousePos.x > getTransform().transformPoint(mIndicators[startVert + 0].position).x
			&& mousePos.x < getTransform().transformPoint(mIndicators[startVert + 9].position).x
			&& mousePos.y > getTransform().transformPoint(mIndicators[startVert + 0].position).y
			&& mousePos.y < getTransform().transformPoint(mIndicators[startVert + 19].position).y;
	}

	void TurretPlacementIndicators::turretPlacementOccupied(unsigned indicator, bool flag)
	{
		const sf::Color color(flag ? sf::Color::Yellow : sf::Color::Green);
		const unsigned startVert = indicator * 32;

		for (unsigned i = startVert; i < startVert + 32; i++)
			mIndicators[i].color = color;
	}


	void TurretPlacementIndicators::setup(const std::array<sf::FloatRect, 3>& windowRects)
	{
		for (unsigned i = 0; i < 3; i++)
		{
			const sf::Vector2f baseWinSize(windowRects[i].width, windowRects[i].height);
			const sf::Vector2f startPos(windowRects[i].left, windowRects[i].top);
			const sf::Vector2f endPos(windowRects[i].width / 3.f, windowRects[i].height / 3.f);
			const float size = 2.f;

			// Top Left
				// Horizontal
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + size), sf::Color::Green));
				// Vertical
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + endPos.y), sf::Color::Green));

			// Top Right
				// Horizontal
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + size), sf::Color::Green));
				// Vertical
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + endPos.y), sf::Color::Green));

			// Bottom Left
				// Horizontal
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + baseWinSize.y - size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + baseWinSize.y - size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + baseWinSize.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + baseWinSize.y),        sf::Color::Green));
				// Vertical
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + baseWinSize.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + baseWinSize.y),            sf::Color::Green));

			// Bottom Right
				// Horizontal
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + baseWinSize.y - size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + baseWinSize.y - size), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + baseWinSize.y),        sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + baseWinSize.y),        sf::Color::Green));
				// Vertical
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + baseWinSize.y),            sf::Color::Green));
			mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + baseWinSize.y),            sf::Color::Green));
		}
	}

	void TurretPlacementIndicators::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (mHoverLayer)
			target.draw(*mHoverLayer, states);
		target.draw(mIndicators, states);
	}

	void TurretPlacementIndicators::handleEvent(const sf::Event& event)
	{
		mShouldCreateTurret = mSelectedIndicator != -1 && event.type == sf::Event::MouseButtonReleased
							&& event.mouseButton.button == sf::Mouse::Left;

		if (mShouldCreateTurret)
			turretPlacementOccupied(mSelectedIndicator, true);
	}

	void TurretPlacementIndicators::update()
	{
		for (unsigned i = 0; i < 3; i++)
			if (hover(i)) {
				if (mSelectedIndicator != i && !mHoverLayer) {
					unsigned startVert = i * 32;
					mSelectedIndicator = i;

					const sf::Color& color(mIndicators[startVert].color);
					mHoverLayer = std::make_unique<sf::VertexArray>(sf::Quads);
					mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert].position),      sf::Color(color.r, color.g, color.b, 50)));
					mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 9].position),  sf::Color(color.r, color.g, color.b, 50)));
					mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 25].position), sf::Color(color.r, color.g, color.b, 50)));
					mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 19].position), sf::Color(color.r, color.g, color.b, 50)));
				}

				return;
			}

		mSelectedIndicator = -1;
		mHoverLayer.reset();
	}
}