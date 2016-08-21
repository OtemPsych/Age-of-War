#include <PYRO/Math.h>

namespace gui
{
	template <typename T, typename K>
	UpgradeButtons<T, K>::Button::Button(const sf::Vector2f& button_size, sf::RenderWindow* window)
		: pyro::gui::Button(button_size, window)
		, overlay(nullptr)
	{
	}

	template <typename T, typename K>
	UpgradeButtons<T, K>::UpgradeButtons(const TData& data, const std::pair<std::vector<sf::Vector2f>, sf::Vector2f>& button_properties,
										 sf::RenderWindow* window)
		: data_(data)
		, window_(window)
		, button_colors_(std::make_pair(sf::Color::Blue, sf::Color(230, 230, 230)))
	{
		setupResources();
		setup(button_properties);
	}

	template <typename T, typename K>
	void UpgradeButtons<T, K>::setup(const std::pair<std::vector<sf::Vector2f>, sf::Vector2f>& button_properties)
	{
		const float radius = 10.f;
		const float margin = 10.f;
		const size_t total_button_vertices = 8;
		const size_t total_button_sets = button_properties.first.size();

		for (size_t i = 0; i < total_button_sets; i++) {
			std::pair<Button*, K> button_set;
			for (size_t j = 0; j < data_[i]->upgrade_stat_types.size(); ++j) {
				auto button(std::make_unique<Button>(sf::Vector2f(radius * 2.f, radius * 2.f), window_));
				button->stat_type = static_cast<data::UnitTurretData::UpgradeStatType>(j);
				button->setTexture(&button_textures_.get(button->stat_type));
				button->setPosition(button_properties.first[i].x + (radius * 2 + margin) * (j * 0.5f),
					                button_properties.first[i].y + button_properties.second.y + radius + 4.f);

				button_set.first = button.get();
				attachChild(std::move(button));
			}
			button_set.second = static_cast<K>(i);
			buttons_.push_back(std::move(button_set));
		}

		//const float radius = 5.f;
		//const float margin = 2.f;
		//const float totalButtonVertices = 8;

		//unsigned short totalButtons = 2;

		//unsigned short totalButtons = std::is_same<T, data::UnitData>::value ? 5 : 3;
		//for (unsigned short i = 0; i < totalButtons; i++) {
		//	mButtons.emplace_back(std::make_pair(pyro::gui::Button(mWindow), sf::VertexArray(sf::TrianglesFan, totalButtonVertices)));

		//	const sf::Vector2f center(radius + spawnButtonSize.x * (i + 0.5f), spawnButtonSize.y + 25.f);

		//	sf::VertexArray& vertices = mButtons[i].first.getVertices();
		//	vertices.append(sf::Vertex(center, sf::Color::Black));
		//	vertices[0].position = center;

		//	for (unsigned short j = 1; j < totalButtonVertices; j++) {
		//		float angle = Math_PI * 2.f * j / (totalButtonVertices - 2);
		//		vertices.append(sf::Vertex(center + sf::Vector2f(cos(angle) * radius, sin(angle) * radius), sf::Color::Black));
		//	}

		//	mButtons[i].second = vertices;
		//}
	}

	template <typename T, typename K>
	void UpgradeButtons<T, K>::setupResources()
	{
		button_textures_.load("Assets/Textures/UpgradeHealth.png", data::UnitTurretData::UpgradeStatType::Health);
		button_textures_.load("Assets/Textures/UpgradeDamage.png", data::UnitTurretData::UpgradeStatType::Damage);
		button_textures_.load("Assets/Textures/UpgradeHealth.png", data::UnitTurretData::UpgradeStatType::Range);
		button_textures_.load("Assets/Textures/UpgradeHealth.png", data::UnitTurretData::UpgradeStatType::Rate);
	}
}