#include "Camera.h"

Camera::Camera(const sf::FloatRect& world_bounds, sf::RenderWindow* window)
	: world_bounds_(world_bounds)
	, window_(window)
	, window_bounds_(0, 0, window->getSize().x, window->getSize().y)
	, scroll_speed_(7.f)
	, scroll_edge_(100.f)
{
}

void Camera::updateCurrent(sf::Time dt)
{
	if (window_bounds_.contains(sf::Mouse::getPosition(*window_))) {
		const sf::Vector2f view_center(window_->getView().getCenter());
		const sf::Vector2i mouse_pos(sf::Mouse::getPosition(*window_));
		const float coords_x = window_->mapPixelToCoords(mouse_pos).x;
		const float half_view_width = window_->getView().getSize().x / 2.f;

		if (coords_x >= view_center.x + half_view_width - scroll_edge_) {
			sf::View new_view(window_->getView());
			if (view_center.x + half_view_width + scroll_speed_ < world_bounds_.width)
				new_view.move(-(window_bounds_.width - mouse_pos.x - scroll_edge_) * scroll_speed_ / 100.f, 0.f);
			else
				new_view.setCenter(world_bounds_.width - half_view_width, view_center.y);

			setPosition(new_view.getCenter().x - half_view_width, 0.f);
			window_->setView(new_view);
		}
		else if (coords_x <= view_center.x - half_view_width + scroll_edge_) {
			sf::View new_view(window_->getView());
			if (view_center.x - half_view_width - scroll_speed_ > world_bounds_.left)
				new_view.move(-(scroll_edge_ - mouse_pos.x) * scroll_speed_ / 100.f, 0.f);
			else
				new_view.setCenter(world_bounds_.left + half_view_width, view_center.y);

			setPosition(new_view.getCenter().x - half_view_width, 0.f);
			window_->setView(new_view);
		}
	}
}