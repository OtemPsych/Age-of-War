#ifndef Mage_H_
#define Mage_H_

#include "Unit.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Mage : public Unit
{
private:
	std::vector<sf::VertexArray> mSpheres;

private:
	void spawnSphere();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Mage(Side side, UnitData& data, const pyro::TextureHolder<Unit::Type>& textures,
		 pyro::SoundPlayer<SoundID>& soundPlayer);
public:
	virtual void attack(Entity& entity);
	virtual void update(sf::Time dt);
};
#endif