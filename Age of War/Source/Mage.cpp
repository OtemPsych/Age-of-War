#include "Mage.h"

#include <PYRO/Math.h>
#include <SFML/Graphics/RenderTarget.hpp>

Mage::Mage(Side side, gStruct::UnitData& data, const pyro::TextureHolder<Unit::Type>& textures,
		   pyro::SoundPlayer<SoundID>& soundPlayer, const sf::Texture* projTexture)
	: Unit(side, data, textures, soundPlayer)
	, mProjectileTexture(projTexture)
{
}

void Mage::spawnSphere()
{
	const float radius = 35.f;

	sf::Vector2f center(15.f, -15.f);

	mSpheres.push_back(sf::VertexArray(sf::TrianglesFan, 32));
	mSpheres.back()[0].position = center;
	mSpheres.back()[0].color = sf::Color::Black;

	for (unsigned i = 1; i < mSpheres.back().getVertexCount(); i++)
	{
		float angle = Math_PI * 2.f * i / (mSpheres.back().getVertexCount() - 2);
		mSpheres.back()[i].position = center + sf::Vector2f(cos(angle) * radius, sin(angle) * radius);
		mSpheres.back()[i].color = i % 5 == 0 ? sf::Color::White : sf::Color::Magenta;
	}

	if (mProjectileTexture)
	{
		sf::Vector2f textureSize(mProjectileTexture->getSize());
		mSpheres.back()[0].texCoords = sf::Vector2f(0.f, 0.f);
		mSpheres.back()[1].texCoords = sf::Vector2f(textureSize.x, 0.f);
		mSpheres.back()[2].texCoords = textureSize;
		mSpheres.back()[3].texCoords = sf::Vector2f(0.f, textureSize.y);
	}
}

void Mage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);

	states.transform *= getTransform();
	if (mProjectileTexture)
		states.texture = mProjectileTexture;

	for (const auto& sphere : mSpheres)
		target.draw(sphere, states);
}

void Mage::attack(Entity& entity)
{
	mAttacking = canAttackTarget(entity);

	if (!mAttackAnimation.isAnimationOngoing())
	{
		spawnSphere();
		mSoundPlayer.play(static_cast<Unit::SoundID>(mType), getPosition(), 15.f);
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