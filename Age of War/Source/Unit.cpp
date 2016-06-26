#include "Unit.h"
#include "Base.h"

#include <PYRO/Math.h>

#include <SFML/Graphics/RenderTarget.hpp>

Unit::Unit(Side side, sf::Font& font, gStruct::UnitData& data,
	       const pyro::TextureHolder<UnitType>& unitTextures, pyro::SoundPlayer<SoundID>& soundPlayer)
	: HealthEntity(side, EntityType::Unit, data.health, unitTextures.get(static_cast<UnitType>(data.unitType)),
			       data.walkRects.front().first)
	, mAttackRange(data.range)
	, mAttackRate(data.rate)
	, mSpeed(data.speed)
	, mDamageDisplayFont(font)
	, mMoving(true)
	, mWalkingAnimation(mSprite, data.walkRects, sf::seconds(0.65f), true)
	, mGeneralUnitType(static_cast<GeneralUnitType>(data.generalUnitType))
	, mUnitType(static_cast<UnitType>(data.unitType))
	, mDamage(data.damage)
	, mSoundPlayer(soundPlayer)
	, mAttacking(false)
	, mAttackAnimation(mSprite, data.attackRects, data.rate, false)
{
	scale(data.scale, data.scale);
}

Unit::~Unit()
{
}

Unit::DamageDisplay::DamageDisplay(sf::FloatRect enemyGBounds, sf::Font& font, unsigned short damage, bool ally)
	: lifetime(sf::seconds(0.75f))
	, acceleration(0.f, -1.8f)
	, velocity(0.f, 30.f)
{
	if (!ally)
		velocity = -velocity;

	// std::max(my half, their half);

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

void Unit::DamageDisplay::operator=(const DamageDisplay& copy)
{
	text = copy.text;
	velocity = copy.velocity;
	acceleration = copy.acceleration;
	lifetime = copy.lifetime;
}

void Unit::updateDamageDisplays(sf::Time dt)
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

bool Unit::enemyInRange(HealthEntity& enemy)
{
	const sf::FloatRect gBounds(getGlobalBounds());
	const sf::FloatRect enemyGBounds(enemy.getGlobalBounds());

	if (mSide == Side::Ally) {
		if (gBounds.left + gBounds.width + mAttackRange >= enemyGBounds.left)
			return true;
	}
	else
		if (gBounds.left - mAttackRange <= enemyGBounds.left + enemyGBounds.width)
			return true;

	return false;
}

void Unit::handleAttackAnimation(HealthEntity& enemy)
{
	if (!mAttackAnimation.isAnimationOngoing())
	{
		if (mUnitType < static_cast<unsigned short>(Unit::SoundID::TypeCount))
			mSoundPlayer.play(static_cast<Unit::SoundID>(mUnitType), getPosition(), 10.f);
		enemy.receiveDamage(mDamage);
		mDamageDisplays.emplace_back(DamageDisplay(enemy.getGlobalBounds(), mDamageDisplayFont, mDamage, mSide == Side::Ally));
		mAttackRate.current = sf::Time::Zero;
		mAttackAnimation.restart();
	}
}

void Unit::drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& damageDisplay : mDamageDisplays)
		target.draw(damageDisplay.text, states);
}

void Unit::attack(HealthEntity& enemy)
{
	if (enemyInRange(enemy)) {
		mMoving = false;
		if (mAttackRate.current >= mAttackRate.original)
			mAttacking = true;
	}
	else
		mMoving = true;

	handleAttackAnimation(enemy);
}

void Unit::update(sf::Time dt)
{
	mAttackRate.current += dt;

	if (!mAttacking && mMoving)
	{
		mWalkingAnimation.update(dt);
		if (mSide == Side::Ally)
			move(mSpeed * dt.asSeconds(), 0.f);
		else
			move(-(mSpeed * dt.asSeconds()), 0.f);
	}
	else if (mAttacking)
		mAttackAnimation.update(dt);

	updateDamageDisplays(dt);
}