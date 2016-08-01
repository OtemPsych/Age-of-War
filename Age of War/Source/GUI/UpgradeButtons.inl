#include <PYRO/Math.h>

namespace gui
{
	template <typename T>
	UpgradeButtons<T>::UpgradeButtons(const TData& data, const sf::Vector2f& spawnButtonSize)
		: mTData(data)
	{
		setup(spawnButtonSize);
	}

	template <typename T>
	void UpgradeButtons<T>::setup(const sf::Vector2f& spawnButtonSize)
	{
		const float radius = 5.f;
		const float margin = 2.f;
		const float totalButtonVertices = 8;

		unsigned short totalButtons = 2;

		unsigned short totalButtons = std::is_same<T, data::UnitData>::value ? 5 : 3;
		for (unsigned short i = 0; i < totalButtons; i++) {
			mButtons.emplace_back(std::make_pair(pyro::gui::Button(mWindow), sf::VertexArray(sf::TrianglesFan, totalButtonVertices)));

			const sf::Vector2f center(radius + spawnButtonSize.x * (i + 0.5f), spawnButtonSize.y + 25.f);

			sf::VertexArray& vertices = mButtons[i].first.getVertices();
			vertices.append(sf::Vertex(center, sf::Color::Black));
			vertices[0].position = center;

			for (unsigned short j = 1; j < totalButtonVertices; j++) {
				float angle = Math_PI * 2.f * j / (totalButtonVertices - 2);
				vertices.append(sf::Vertex(center + sf::Vector2f(cos(angle) * radius, sin(angle) * radius), sf::Color::Black));
			}

			mButtons[i].second = vertices;
		}
	}
}