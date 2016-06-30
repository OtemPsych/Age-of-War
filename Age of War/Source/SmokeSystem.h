#ifndef SmokeSystem_H_
#define SmokeSystem_H_

#include <PYRO/ParticleSystem.h>

#include <SFML/System/NonCopyable.hpp>

class SmokeSystem : public sf::Drawable, private sf::NonCopyable
{
private:
	std::vector<pyro::ParticleSystem> mSmokes;
	sf::Texture                       mSmokeTexture;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	SmokeSystem();
public:
	void addEmitterPosition(const sf::Vector2f& pos);
	void update(sf::Time dt);
};
#endif