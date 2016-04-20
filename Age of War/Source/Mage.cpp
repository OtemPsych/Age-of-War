#include "Mage.h"

#include <SFML/Graphics/RenderTarget.hpp>

Mage::Mage(Side side, gStruct::UnitData& data, const pyro::TextureHolder<Unit::Type>& textures,
		   pyro::SoundPlayer<SoundID>& soundPlayer)
	: Unit(side, data, textures, soundPlayer)
{
}

void Mage::spawnSphere()
{
	const float PI = 3.141592654f;
	const float radius = 35.f;

	sf::Vector2f pos(getPosition());
	sf::Vector2f center(getGlobalBounds().width * 2, 15.f);

	mSpheres.push_back(sf::VertexArray(sf::TrianglesFan, 32));
	mSpheres.back()[0].position = center;
	mSpheres.back()[0].color = sf::Color::Black;

	for (unsigned i = 1; i < mSpheres.back().getVertexCount(); i++)
	{
		float angle = PI * 2.f * i / (mSpheres.back().getVertexCount() - 2);
		mSpheres.back()[i].position = center + sf::Vector2f(cos(angle) * radius, sin(angle) * radius);
		mSpheres.back()[i].color = i % 5 == 0 ? sf::Color::White : sf::Color::Magenta;
	}
}

void Mage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);

	states.transform *= getTransform();
	for (const auto& sphere : mSpheres)
		target.draw(sphere, states);
}

void Mage::attack(Entity& entity)
{
	mAttacking = canAttackTarget(entity);

	if (!mAttackAnimation.isAnimationOngoing())
	{
		spawnSphere();
		mSoundPlayer.play(SoundID::MageAttack, getPosition(), 5.f);
		mAttackAnimation.restart();
	}

	for (unsigned i = 0; i < mSpheres.size(); i++)
	{
		if (entity.getGlobalBounds().contains(getTransform().transformPoint(mSpheres[i][0].position)))
		{
			entity.reduceHealth(mDamage);
			mSpheres.erase(mSpheres.begin() + i);
		}
	}
}

void Mage::update(sf::Time dt)
{
	for (auto& sphere : mSpheres)
		for (unsigned i = 0; i < sphere.getVertexCount(); i++)
		{
			sf::Vector2f currPos(sphere[i].position);
			sphere[i].position += sf::Vector2f(700.f * dt.asSeconds(), 0.f);
		}

	Unit::update(dt);
}