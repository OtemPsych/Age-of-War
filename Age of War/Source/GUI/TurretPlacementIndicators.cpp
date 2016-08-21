#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include <PYRO/Utils.h>

#include "TurretPlacementIndicators.h"

namespace gui
{
	TurretPlacementIndicators::TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& window_rects,
		                                                 sf::RenderWindow* window)
		: VertexArrayNode(sf::Vector2f(0.f, 0.f))
		, window_(window)
		, hover_layer_(nullptr)
		, selected_indicator_(-1)
		, should_create_turret_(false)
	{
		vertices_.clear();
		init(window_rects);
	}

	int TurretPlacementIndicators::getTurretIndicator()
	{
		if (selected_indicator_ != -1)
			occupyTurretPlacement(selected_indicator_, true);

		return selected_indicator_;
	}

	void TurretPlacementIndicators::updateCurrent(sf::Time dt)
	{
		for (unsigned i = 0; i < 3; i++)
			if (hover(i)) {
				if (selected_indicator_ != i) {
					unsigned start_vert = i * 32;
					selected_indicator_ = i;

					const sf::Color& color = vertices_[start_vert].color;
					hover_layer_->setFillColor(sf::Color(color.r, color.g, color.b, 50));
					hover_layer_->setPosition(vertices_[start_vert].position);

					hover_layer_->activateDrawing(ActivationTarget::Current, true);
				}

				return;
			}

		selected_indicator_ = -1;
		hover_layer_->activateDrawing(ActivationTarget::Current, false);
	}

	bool TurretPlacementIndicators::hover(unsigned indicator)
	{
		const unsigned start_vert = indicator * 32;
		const sf::Vector2f mouse_pos(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
		const sf::Transform world_transform(getWorldTransform());

		return mouse_pos.x > world_transform.transformPoint(vertices_[start_vert + 0].position).x
			&& mouse_pos.x < world_transform.transformPoint(vertices_[start_vert + 9].position).x
			&& mouse_pos.y > world_transform.transformPoint(vertices_[start_vert + 0].position).y
			&& mouse_pos.y < world_transform.transformPoint(vertices_[start_vert + 19].position).y;
	}

	void TurretPlacementIndicators::occupyTurretPlacement(unsigned indicator, bool flag)
	{
		const sf::Color color(flag ? sf::Color::Yellow : sf::Color::Green);
		const unsigned start_vert = indicator * 32;

		for (unsigned i = start_vert; i < start_vert + 32; i++)
			vertices_[i].color = color;
	}

	void TurretPlacementIndicators::init(const std::array<sf::FloatRect, 3>& window_rects)
	{
		// Init Indicators
		for (unsigned i = 0; i < 3; i++) {
			const sf::Vector2f base_win_size(window_rects[i].width, window_rects[i].height);
			const sf::Vector2f start_pos(window_rects[i].left, window_rects[i].top);
			const sf::Vector2f end_pos(window_rects[i].width / 3.f, window_rects[i].height / 3.f);
			const float size = 2.f;

			// Top Left
				// Horizontal
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,             start_pos.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + end_pos.x, start_pos.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + end_pos.x, start_pos.y + size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,             start_pos.y + size), sf::Color::Green));
				// Vertical
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,        start_pos.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + size, start_pos.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + size, start_pos.y + end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,        start_pos.y + end_pos.y), sf::Color::Green));

			// Top Right
				// Horizontal
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - end_pos.x, start_pos.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,             start_pos.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,             start_pos.y + size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - end_pos.x, start_pos.y + size), sf::Color::Green));
				// Vertical
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - size, start_pos.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,        start_pos.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,        start_pos.y + end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - size, start_pos.y + end_pos.y), sf::Color::Green));

			// Bottom Left
				// Horizontal
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,             start_pos.y + base_win_size.y - size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + end_pos.x, start_pos.y + base_win_size.y - size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + end_pos.x, start_pos.y + base_win_size.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,             start_pos.y + base_win_size.y),        sf::Color::Green));
				// Vertical
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,        start_pos.y + base_win_size.y - end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + size, start_pos.y + base_win_size.y - end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + size, start_pos.y + base_win_size.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x,        start_pos.y + base_win_size.y),             sf::Color::Green));

			// Bottom Right
				// Horizontal
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - end_pos.x, start_pos.y + base_win_size.y - size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,             start_pos.y + base_win_size.y - size), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,             start_pos.y + base_win_size.y),        sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - end_pos.x, start_pos.y + base_win_size.y),        sf::Color::Green));
				// Vertical
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - size, start_pos.y + base_win_size.y - end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,        start_pos.y + base_win_size.y - end_pos.y), sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x,        start_pos.y + base_win_size.y),             sf::Color::Green));
			vertices_.append(sf::Vertex(sf::Vector2f(start_pos.x + base_win_size.x - size, start_pos.y + base_win_size.y),             sf::Color::Green));
		}

		// Init Hover Layer
		const sf::Vector2f base_win_size(window_rects[0].width, window_rects[0].height);
		auto hover_layer(std::make_unique<pyro::VertexArrayNode>(base_win_size));
		hover_layer->activateEventHandling(ActivationTarget::Both, false);
		hover_layer->activateUpdating(ActivationTarget::Both, false);
		hover_layer->activateDrawing(ActivationTarget::Both, false);

		hover_layer_ = hover_layer.get();
		attachChild(std::move(hover_layer));

		setOriginFlags(pyro::utils::OriginFlag::Center);
	}

	//TurretPlacementIndicators::TurretPlacementIndicators(const std::array<sf::FloatRect, 3>& windowRects,
	//	                                                 sf::RenderWindow& window)
	//	: mIndicators(sf::Quads)
	//	, mHoverLayer(nullptr)
	//	, mSelectedIndicator(-1)
	//	, mShouldCreateTurret(false)
	//	, mWindow(window)
	//{
	//	setup(windowRects);
	//}

	//bool TurretPlacementIndicators::hover(unsigned indicator)
	//{
	//	const unsigned startVert = indicator * 32;
	//	const sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow));
	//
	//	return mousePos.x > getTransform().transformPoint(mIndicators[startVert + 0].position).x
	//		&& mousePos.x < getTransform().transformPoint(mIndicators[startVert + 9].position).x
	//		&& mousePos.y > getTransform().transformPoint(mIndicators[startVert + 0].position).y
	//		&& mousePos.y < getTransform().transformPoint(mIndicators[startVert + 19].position).y;
	//}

	//void TurretPlacementIndicators::turretPlacementOccupied(unsigned indicator, bool flag)
	//{
	//	const sf::Color color(flag ? sf::Color::Yellow : sf::Color::Green);
	//	const unsigned startVert = indicator * 32;
	//
	//	for (unsigned i = startVert; i < startVert + 32; i++)
	//		mIndicators[i].color = color;
	//}

	//void TurretPlacementIndicators::setup(const std::array<sf::FloatRect, 3>& windowRects)
	//{
	//	for (unsigned i = 0; i < 3; i++)
	//	{
	//		const sf::Vector2f baseWinSize(windowRects[i].width, windowRects[i].height);
	//		const sf::Vector2f startPos(windowRects[i].left, windowRects[i].top);
	//		const sf::Vector2f endPos(windowRects[i].width / 3.f, windowRects[i].height / 3.f);
	//		const float size = 2.f;
	//
	//		// Top Left
	//			// Horizontal
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + size), sf::Color::Green));
	//			// Vertical
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + endPos.y), sf::Color::Green));
	//
	//		// Top Right
	//			// Horizontal
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + size), sf::Color::Green));
	//			// Vertical
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + endPos.y), sf::Color::Green));
	//
	//		// Bottom Left
	//			// Horizontal
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + baseWinSize.y - size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + baseWinSize.y - size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + endPos.x, startPos.y + baseWinSize.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,            startPos.y + baseWinSize.y),        sf::Color::Green));
	//			// Vertical
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + size, startPos.y + baseWinSize.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x,        startPos.y + baseWinSize.y),            sf::Color::Green));
	//
	//		// Bottom Right
	//			// Horizontal
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + baseWinSize.y - size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + baseWinSize.y - size), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,            startPos.y + baseWinSize.y),        sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - endPos.x, startPos.y + baseWinSize.y),        sf::Color::Green));
	//			// Vertical
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + baseWinSize.y - endPos.y), sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x,        startPos.y + baseWinSize.y),            sf::Color::Green));
	//		mIndicators.append(sf::Vertex(sf::Vector2f(startPos.x + baseWinSize.x - size, startPos.y + baseWinSize.y),            sf::Color::Green));
	//	}
	//}

	//void TurretPlacementIndicators::draw(sf::RenderTarget& target, sf::RenderStates states) const
	//{
	//	if (mHoverLayer) {
	//		target.draw(*mHoverLayer, states);
	//	}
	//	target.draw(mIndicators, states);
	//}

	//bool TurretPlacementIndicators::handleEvent(const sf::Event& event)
	//{
	//	mShouldCreateTurret = mSelectedIndicator != -1 && event.type == sf::Event::MouseButtonReleased
	//						&& event.mouseButton.button == sf::Mouse::Left;
	//
	//	if (mShouldCreateTurret) {
	//		turretPlacementOccupied(mSelectedIndicator, true);
	//		return true;
	//	}
	//
	//	return false;
	//}

	//void TurretPlacementIndicators::update()
	//{
	//	for (unsigned i = 0; i < 3; i++)
	//		if (hover(i)) {
	//			if (mSelectedIndicator != i && !mHoverLayer) {
	//				unsigned startVert = i * 32;
	//				mSelectedIndicator = i;
	//
	//				const sf::Color& color(mIndicators[startVert].color);
	//				mHoverLayer = std::make_unique<sf::VertexArray>(sf::Quads);
	//				mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert].position),      sf::Color(color.r, color.g, color.b, 50)));
	//				mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 9].position),  sf::Color(color.r, color.g, color.b, 50)));
	//				mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 25].position), sf::Color(color.r, color.g, color.b, 50)));
	//				mHoverLayer->append(sf::Vertex(sf::Vector2f(mIndicators[startVert + 19].position), sf::Color(color.r, color.g, color.b, 50)));
	//			}
	//
	//			return;
	//		}
	//
	//	mSelectedIndicator = -1;
	//	mHoverLayer.reset();
	//}
}