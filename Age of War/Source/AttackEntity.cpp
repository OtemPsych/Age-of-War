#include "AttackEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>

AttackEntity::AttackEntity(Side side, EntityType entityType, float attackRange, const sf::Time& attackRate,
	                       unsigned short damage, sf::Font& damageDisplayFont, const sf::Texture& texture)
	: Entity(side, entityType, texture)
	, mDamageDisplayFont(damageDisplayFont)
	, mAttackRange(attackRange)
	, mAttackRate(attackRate)
	, mDamage(damage)
	, mAttacking(false)
{
}

AttackEntity::AttackEntity(Side side, EntityType entityType, float attackRange, const sf::Time& attackRate,
	                       unsigned short damage, sf::Font& damageDisplayFont, const sf::Texture& texture, sf::IntRect rect)
	: Entity(side, entityType, texture, rect)
	, mDamageDisplayFont(damageDisplayFont)
	, mAttackRange(attackRange)
	, mAttackRate(attackRate)
	, mDamage(damage)
	, mAttacking(false)
{
}

AttackEntity::~AttackEntity()
{
}

AttackEntity::DamageDisplay::DamageDisplay(sf::FloatRect enemyGBounds, sf::Font& font, unsigned short damage, bool ally)
	: lifetime(sf::seconds(0.75f))
	, acceleration(0.f, -1.8f)
	, velocity(0.f, 30.f)
{
	if (!ally)
		velocity = -velocity;

	text.setFont(font);
	text.setString(std::to_string(damage));
	text.setCharacterSize(14);
	text.setTextColor(sf::Color(255, 51, 51));
	text.activateShadow(true);
	text.setShadowOffset(1.5f, 1.5f);
	text.setShadowColor(sf::Color::Black);
	text.setOriginFlags(pyro::utils::Center);
	float minPosX = enemyGBounds.left + text.getGlobalBounds().width / 2.f;
	float maxPosX = enemyGBounds.left + enemyGBounds.width - text.getGlobalBounds().width / 2.f;
	text.setPosition(rand() / (RAND_MAX / (maxPosX - minPosX)) + minPosX, enemyGBounds.top);
}

void AttackEntity::DamageDisplay::operator=(const DamageDisplay& copy)
{
	text = copy.text;
	velocity = copy.velocity;
	acceleration = copy.acceleration;
	lifetime = copy.lifetime;
}

void AttackEntity::addDamageDisplay(const sf::FloatRect& enemyGBounds)
{
	mDamageDisplays.emplace_back(enemyGBounds, mDamageDisplayFont, mDamage, mSide == Side::Ally);
}

void AttackEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
}

void AttackEntity::drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& damageDisplay : mDamageDisplays)
		target.draw(damageDisplay.text, states);
}

void AttackEntity::update(sf::Time dt)
{
	for (unsigned i = 0; i < mDamageDisplays.size(); i++) {
		mDamageDisplays[i].text.move(mDamageDisplays[i].velocity * dt.asSeconds());
		mDamageDisplays[i].velocity += mDamageDisplays[i].acceleration;

		mDamageDisplays[i].lifetime.current -= dt;
		sf::Uint8 alphaColor = static_cast<sf::Uint8>(255 * std::max(mDamageDisplays[i].lifetime.current / mDamageDisplays[i].lifetime.original, 0.f));

		const sf::Color& textColor = mDamageDisplays[i].text.getTextColor();
		mDamageDisplays[i].text.setTextColor(sf::Color(textColor.r, textColor.g, textColor.b, alphaColor));

		const sf::Color& shadowColor = *mDamageDisplays[i].text.getShadowColor();
		mDamageDisplays[i].text.setShadowColor(sf::Color(shadowColor.r, shadowColor.g, shadowColor.b, alphaColor));

		if (mDamageDisplays[i].lifetime.current <= sf::Time::Zero)
			mDamageDisplays.erase(mDamageDisplays.begin() + i);
	}
}