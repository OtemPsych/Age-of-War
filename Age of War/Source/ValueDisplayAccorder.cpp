#include "ValueDisplayAccorder.h"

#include <SFML/Graphics/RenderTarget.hpp>

// Value Display
ValueDisplayAccorder::ValueDisplay::ValueDisplay(const sf::Vector2f& starting_velocity,
	                            const sf::Vector2f& acceleration, const sf::Font& font,
	                            sf::Time original_lifetime, unsigned value,
								pyro::SceneNode* owner_node)
	: text_(nullptr)
	, velocity_(starting_velocity)
	, acceleration_(acceleration)
	, lifetime_(original_lifetime)
{
	auto text(std::make_unique<pyro::Text>());
	text->setFont(font);
	text->setCharacterSize(14);
	text->setString(std::to_string(value));
	text->setTextColor(sf::Color(255, 51, 51));
	text->activateShadow(true);
	text->setShadowOffset(1.5f, 1.5f);
	text->setShadowColor(sf::Color::Black);
	text->setOriginFlags(pyro::utils::OriginFlag::CenterX | pyro::utils::OriginFlag::Bottom);
	sf::FloatRect owner_lbounds(owner_node->getLocalBounds());
	text->setPosition(owner_lbounds.left + owner_lbounds.width / 2.f, owner_lbounds.top);

	text_ = text.get();
	attachChild(std::move(text));
}

bool ValueDisplayAccorder::ValueDisplay::isDestroyed() const
{
	return lifetime_.current <= sf::Time::Zero;
}

void ValueDisplayAccorder::ValueDisplay::updateCurrent(sf::Time dt)
{
	move(velocity_ * dt.asSeconds());
	velocity_ += acceleration_;

	lifetime_.current -= dt;

	sf::Uint8 alpha_color = static_cast<sf::Uint8>(255 * std::max(lifetime_.current / lifetime_.original, 0.f));
	const sf::Color& text_color = text_->getTextColor();
	text_->setTextColor(sf::Color(text_color.r, text_color.g, text_color.b, alpha_color));

	const sf::Color& shadow_color = *text_->getShadowColor();
	text_->setShadowColor(sf::Color(shadow_color.r, shadow_color.g, shadow_color.b, alpha_color));
}

// ValueDisplayAccorder
ValueDisplayAccorder::ValueDisplayAccorder(const sf::Vector2f& starting_velocity, const sf::Vector2f& acceleration,
	                                       const sf::Font& font, sf::Time lifetime)
	: value_velocity_(starting_velocity)
	, value_acceleration_(acceleration)
	, value_lifetime_(lifetime)
	, font_(font)
{
}

void ValueDisplayAccorder::addValueDisplay(unsigned short value, pyro::SceneNode* owner_node)
{
	auto value_display(std::make_unique<ValueDisplay>(value_velocity_, value_acceleration_, font_,
		                                              value_lifetime_, value, owner_node));
	owner_node->attachChild(std::move(value_display));
}