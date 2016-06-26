#include "ValueDisplays.h"

#include <SFML/Graphics/RenderTarget.hpp>

ValueDisplays::ValueDisplays(const sf::Vector2f& velocity, const sf::Vector2f& acceleration,
	                         const sf::Time& lifetime, sf::Font& font)
	: mValueVelocity(velocity)
	, mValueAcceleration(acceleration)
	, mValueLifetime(lifetime)
	, mFont(font)
{
}

ValueDisplays::ValueDisplay::ValueDisplay(ValueDisplays* owner, sf::FloatRect gBounds,
	                                      sf::FloatRect enemyGBounds, unsigned value)
	: velocity(owner->mValueAcceleration)
	, acceleration(owner->mValueAcceleration)
	, lifetime(owner->mValueLifetime)
{
	text.setFont(owner->mFont);
	text.setString(std::to_string(value));
	text.setCharacterSize(14);
	text.setTextColor(sf::Color(255, 51, 51));
	text.activateShadow(true);
	text.setShadowOffset(1.5f, 1.5f);
	text.setShadowColor(sf::Color::Black);
	text.setOriginFlags(pyro::utils::Center);
	text.setPosition(enemyGBounds.left + enemyGBounds.width / 2.f, std::max(gBounds.top, enemyGBounds.top));
}

void ValueDisplays::ValueDisplay::operator=(const ValueDisplay& copy)
{
	text = copy.text;
	velocity = copy.velocity;
	acceleration = copy.acceleration;
	lifetime = copy.lifetime;
}

void ValueDisplays::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& valueDisplay : mValueDisplays)
		target.draw(valueDisplay.text, states);
}

void ValueDisplays::addValueDisplay(sf::FloatRect gBounds, sf::FloatRect enemyGBounds, unsigned value)
{
	mValueDisplays.emplace_back(this, gBounds, enemyGBounds, value);
}

void ValueDisplays::update(sf::Time dt)
{
	for (unsigned i = 0; i < mValueDisplays.size(); i++) {
		mValueDisplays[i].text.move(mValueDisplays[i].velocity * dt.asSeconds());
		mValueDisplays[i].velocity += mValueDisplays[i].acceleration;

		mValueDisplays[i].lifetime.current -= dt;
		sf::Uint8 alphaColor = static_cast<sf::Uint8>(255 * std::max(mValueDisplays[i].lifetime.current / mValueDisplays[i].lifetime.original, 0.f));

		const sf::Color& textColor = mValueDisplays[i].text.getTextColor();
		mValueDisplays[i].text.setTextColor(sf::Color(textColor.r, textColor.g, textColor.b, alphaColor));

		const sf::Color& shadowColor = *mValueDisplays[i].text.getShadowColor();
		mValueDisplays[i].text.setShadowColor(sf::Color(shadowColor.r, shadowColor.g, shadowColor.b, alphaColor));

		if (mValueDisplays[i].lifetime.current <= sf::Time::Zero)
			mValueDisplays.erase(mValueDisplays.begin() + i);
	}
}