#include "StatTooltip.h"
#include "../Tools/StringFunctions.h"

namespace gui
{
	StatTooltip::StatTooltip(const std::vector<data::UnitTurretData::StatType>& stat_types)
		: VertexArrayNode(sf::Vector2f(220.f, 110.f))
		, title_(nullptr)
		, cost_(nullptr)
	{
		init(stat_types);
	}

	void StatTooltip::changeStatsDisplayed(const data::UnitTurretData* data)
	{
		if (data->name != title_->getString()) {
			const data::UnitData* unit_data = dynamic_cast<const data::UnitData*>(data);
			const float padding = 6.f;

			// Title
			title_->setString(data->name);
			title_->setPosition(padding, padding);

			// Cost
			cost_->setString(std::to_string(data->cost) + "g");
			cost_->setPosition(vertices_[1].position.x - padding, padding);

			// Stats
				// Init Texts
			size_t itr = 0;
			if (unit_data)
				stats_[itr++].first->setString(strFunc::getPrecisionString(unit_data->health.value.current, 0));
			stats_[itr++].first->setString(strFunc::getPrecisionString(data->damage.value.current, 0));
			stats_[itr++].first->setString(data->range.value.current == 15 ? "/" : strFunc::getPrecisionString(data->range.value.current, 0));
			stats_[itr++].first->setString(strFunc::getPrecisionString(data->rate.value.current.asSeconds(), 1) + "s");
			if (unit_data)
				stats_[itr].first->setString(strFunc::getPrecisionString(unit_data->spawn_time.asSeconds(), 1) + "s");

				// Find Largest Width
			float largest_width = stats_.front().first->getGlobalBounds().width;
			for (size_t i = 1; i < stats_.size(); ++i) {
				float new_width = stats_[i].first->getGlobalBounds().width;
				if (new_width > largest_width)
					largest_width = new_width;
			}

				// Set Text Positions
			float previous_height = 25.f;
			for (auto& text : stats_) {
				float text_height = text.first->getGlobalBounds().height;
				text.first->setPosition(largest_width + padding, text_height / 2.f + previous_height + padding);
				text.second->setPosition(largest_width + padding + 6.f, text_height / 2.f + previous_height + padding);
				previous_height = text.first->getPosition().y + text_height / 2.f;
			}

			// Adjust Overlay to Fit Stats
			vertices_[2].position = sf::Vector2f(vertices_[2].position.x, previous_height - padding);
			vertices_[3].position = sf::Vector2f(vertices_[3].position.x, previous_height - padding);
		}
	}

	void StatTooltip::init(const std::vector<data::UnitTurretData::StatType>& stat_types)
	{
		using pyro::utils::OriginFlag;

		font_.loadFromFile("Assets/Fonts/Menu.ttf");

		// Init Overlay
		setFillColor(sf::Color(0, 0, 0, 200));
		setOriginFlags(OriginFlag::CenterX | OriginFlag::Top);

		// Init Title
		auto title(std::make_unique<pyro::Text>());
		title->setFont(font_);
		title->setCharacterSize(20);
		title->setStyle(sf::Text::Style::Bold);
		title->setTextColor(sf::Color(0, 172, 230));
		title->setOriginFlags(OriginFlag::Left | OriginFlag::Top);

		title_ = title.get();
		attachChild(std::move(title));

		// Init Cost
		auto cost(std::make_unique<pyro::Text>());
		cost->setFont(font_);
		cost->setCharacterSize(18);
		cost->setStyle(sf::Text::Style::Bold);
		cost->setTextColor(sf::Color(255, 255, 77));
		cost->setOriginFlags(OriginFlag::Right | OriginFlag::Top);

		cost_ = cost.get();
		attachChild(std::move(cost));

		// Init Stats
		for (size_t i = 0; i < stat_types.size(); ++i) {
			auto value_text(std::make_unique<pyro::Text>());
			value_text->setFont(font_);
			value_text->setCharacterSize(14);
			value_text->setTextColor(sf::Color(102, 255, 102));
			value_text->setOriginFlags(OriginFlag::Right | OriginFlag::CenterY);

			auto stat_text(std::make_unique<pyro::Text>());
			stat_text->setFont(font_);
			stat_text->setCharacterSize(14);
			stat_text->setOriginFlags(OriginFlag::Left | OriginFlag::CenterY);

			stats_.emplace_back(std::make_pair(value_text.get(), stat_text.get()));
			attachChild(std::move(value_text));
			attachChild(std::move(stat_text));
		}

			// Init Static Stat Texts
		size_t itr = 0;
		if (std::find(stat_types.begin(), stat_types.end(), data::UnitTurretData::StatType::Health) != stat_types.end())
			stats_[itr++].second->setString("Health");
		stats_[itr++].second->setString("Damage");
		stats_[itr++].second->setString("Range");
		stats_[itr++].second->setString("Rate");
		if (std::find(stat_types.begin(), stat_types.end(), data::UnitTurretData::StatType::SpawnTime) != stat_types.end())
			stats_[itr].second->setString("Spawn Time");
	}
}