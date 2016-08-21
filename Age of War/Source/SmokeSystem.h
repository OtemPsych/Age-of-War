#ifndef SmokeSystem_H_
#define SmokeSystem_H_

#include <PYRO/SpriteNode.h>
#include <PYRO/ParticleSystem.h>

class SmokeSystem : public pyro::SceneNode, private sf::NonCopyable
{
public:
	SmokeSystem();
public:
	void addEmitterPosition(const sf::Vector2f& pos);
private:
	virtual void updateCurrent(sf::Time dt) override;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<pyro::ParticleSystem> smokes_;
	sf::Texture                       smoke_texture_;
};
#endif