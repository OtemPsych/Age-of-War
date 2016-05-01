#ifndef Mage_H_
#define Mage_H_

#include "Unit.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Mage : public Unit
{
private:
	std::vector<sf::VertexArray> mSpheres;
	const sf::Texture*           mProjectileTexture;

private:
	void spawnSphere();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Mage(Side side, gStruct::UnitData& data, const pyro::TextureHolder<Unit::Type>& textures,
		 pyro::SoundPlayer<SoundID>& soundPlayer, const sf::Texture* projTexture);
public:
	virtual void attack(Entity& entity);
	virtual void update(sf::Time dt);
};
#endif