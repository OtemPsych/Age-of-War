#include "ValueDisplayManager.h"
#include "DataTables.h"

ValueDisplayManager::ValueDisplay::ValueDisplay(const sf::FloatRect& gbounds,
	                                            const sf::FloatRect& enemy_gbounds, unsigned value,
	                                            data::ValueDisplayData* value_display_data)
	: text(nullptr)
	, velocity(value_display_data->starting_velocity)
	, lifetime(value_display_data->lifetime)
{
	auto text(std::make_unique<pyro::Text>());
	text->setFont(*value_display_data->font);
	text->setString(std::to_string(value));
	text->setCharacterSize(14);
	text->setTextColor(sf::Color(255, 51, 51));
	text->activateShadow(true);
	text->setShadowOffset(1.5f, 1.5f);
	text->setShadowColor(sf::Color::Black);
	text->setOriginFlags(pyro::utils::OriginFlag::CenterX | pyro::utils::OriginFlag::Bottom);
	text->setPosition(enemy_gbounds.left + enemy_gbounds.width / 2.f,
		              std::max(gbounds.top, enemy_gbounds.top));

	this->text = text.get();
	attachChild(std::move(text));
}

ValueDisplayManager::ValueDisplayManager(data::ValueDisplayData* value_display_data)
	: value_display_data_(value_display_data)
{
}

void ValueDisplayManager::addValueDisplay(const sf::FloatRect& lbounds,
	                                      const sf::FloatRect& enemy_lbounds, unsigned value)
{
	auto value_display(std::make_unique<ValueDisplay>(lbounds, enemy_lbounds,
		                                              value, value_display_data_));
	value_displays_.push_back(value_display.get());
	attachChild(std::move(value_display));
}


void ValueDisplayManager::updateCurrent(sf::Time dt)
{
	for (size_t i = 0; i < value_displays_.size(); ++i) {
		if ((value_displays_[i]->lifetime -= dt) <= sf::Time::Zero) {
			detachChild(*value_displays_[i]);
			value_displays_.erase(value_displays_.begin() + i);
			continue;
		}

		value_displays_[i]->text->move(value_displays_[i]->velocity * dt.asSeconds());
		value_displays_[i]->velocity += value_display_data_->acceleration;

		sf::Uint8 alpha_color = static_cast<sf::Uint8>(255 * std::max(value_displays_[i]->lifetime / value_display_data_->lifetime, 0.f));
		const sf::Color& text_color = value_displays_[i]->text->getTextColor();
		value_displays_[i]->text->setTextColor(sf::Color(text_color.r, text_color.g, text_color.b, alpha_color));

		const sf::Color& shadow_color = *value_displays_[i]->text->getShadowColor();
		value_displays_[i]->text->setShadowColor(sf::Color(shadow_color.r, shadow_color.g, shadow_color.b, alpha_color));
	}
}