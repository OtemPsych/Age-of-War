#include "SmokeSystem.h"
#include "Animation.h"
#include "DataTables.h"
#include "Tools/TextureDataReader.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

SmokeSystem::SmokeSystem()
{
	smoke_texture_.loadFromFile("Assets/Textures/Smoke.png");
}

void SmokeSystem::addEmitterPosition(const sf::Vector2f& pos)
{
	for (const auto& smoke : smokes_)
		if (smoke.getEmitterPosition() == pos)
			return;

	smokes_.emplace_back(sf::Quads, 172);
	smokes_.back().setEmitterPosition(pos);
	smokes_.back().activateEmitter(true);
	smokes_.back().setTexture(&smoke_texture_);
	sf::Vector2f textureSize(static_cast<sf::Vector2f>(smoke_texture_.getSize()));
	smokes_.back().setInitializer([&textureSize]()
	{
		pyro::Particle particle;

		float speed = 80.f;
		float minAngle = -105.f;
		float maxAngle = -75.f;
		float angle = pyro::math::toRadians(std::rand() / (RAND_MAX / (maxAngle - minAngle)) + minAngle);
		particle.velocity = sf::Vector2f(cos(angle) * speed, sin(angle) * speed);

		float minLifetime = 0.82f;
		float maxLifetime = 1.34f;
		particle.lifetime = sf::seconds(std::rand() / (RAND_MAX / (maxLifetime - minLifetime)) + minLifetime);

		particle.size = sf::Vector2f(25.f, 25.f);

		sf::Color color;
		switch (rand() % 6) 
		{
		case 0:
			color = sf::Color(77, 77, 77);
			break;
		case 1:
			color = sf::Color(89, 89, 89);
			break;
		case 2:
			color = sf::Color(102, 102, 102);
			break;
		case 3:
			color = sf::Color(115, 115, 115);
			break;
		case 4:
			color = sf::Color(128, 128, 128);
			break;
		case 5:
			color = sf::Color(140, 140, 140);
		}
		particle.color = color;

		return std::move(particle);
	});
	smokes_.back().setAffector([](pyro::Particle& particle, sf::Time dt)
	{
		particle.position += particle.velocity * dt.asSeconds();
	});
}

void SmokeSystem::updateCurrent(sf::Time dt)
{
	for (auto& smoke : smokes_)
		smoke.update(dt);
}

void SmokeSystem::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& smoke : smokes_)
		target.draw(smoke, states);
}