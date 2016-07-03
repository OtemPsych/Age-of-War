#include "SmokeSystem.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

SmokeSystem::SmokeSystem()
{
	mSmokeTexture.loadFromFile("Assets/Textures/Smoke.png");
}

void SmokeSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& smoke : mSmokes)
		target.draw(smoke, states);
}

void SmokeSystem::addEmitterPosition(const sf::Vector2f& pos)
{
	for (const auto& smoke : mSmokes)
		if (smoke.getEmitterPosition() == pos)
			return;

	mSmokes.emplace_back(sf::Quads, 172);
	mSmokes.back().setEmitterPosition(pos);
	mSmokes.back().activateEmitter(true);
	mSmokes.back().setTexture(&mSmokeTexture);
	sf::Vector2f textureSize(static_cast<sf::Vector2f>(mSmokeTexture.getSize()));
	mSmokes.back().setInitializer([&textureSize]()
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
	mSmokes.back().setAffector([](pyro::Particle& particle, sf::Time dt)
	{
		particle.position += particle.velocity * dt.asSeconds();
	});
}

void SmokeSystem::update(sf::Time dt)
{
	for (auto& smoke : mSmokes)
		smoke.update(dt);
}