#ifndef Camera_H_
#define Camera_H_

#include <SFML/Graphics/RenderWindow.hpp>

#include <PYRO/SceneNode.h>

class Camera : public pyro::SceneNode, private sf::NonCopyable
{
public:
	Camera(const sf::FloatRect& world_bounds, sf::RenderWindow* window);
private:
	virtual void updateCurrent(sf::Time dt) override;

private:
	const sf::FloatRect& world_bounds_;
	sf::RenderWindow*    window_;

	const sf::IntRect    window_bounds_;
	const float          scroll_speed_;
	const float          scroll_edge_;
};
#endif