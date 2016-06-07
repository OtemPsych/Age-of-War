#include "../Unit.h"

#include <SFML/Graphics/RenderStates.hpp>

namespace gui
{
	template <typename T, typename K>
	SpawnButtons<T, K>::SpawnButtons(const TData& data, sf::RenderWindow& window,
		                             const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize)
		: mButtonColors(std::make_pair(sf::Color(0, 128, 255), sf::Color(140, 140, 140)))
		, mButtonOverlay(sf::Quads, 4)
		, mStatTooltip(nullptr)
		, mTData(data)
		, mWindow(window)
	{
		setup(textures, buttonSize);
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::setup(const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize)
	{
		const float thickness = 3.f;
		const float margin = thickness + 12.f;
		const float totalButtonSizeX = buttonSize.x + margin;

		sf::Vector2f buttonPos;
		if (std::is_same<K, Unit::UnitType>::value)
			buttonPos = sf::Vector2f(mWindow.getSize().x / 2.f - mTData.size() / 2.f * totalButtonSizeX,
			                         buttonSize.y / 2.f + margin);
		else
			buttonPos = sf::Vector2f(mWindow.getSize().x / 4.f - mTData.size() / 2.f * totalButtonSizeX,
			                         buttonSize.y / 2.f + margin);

		for (unsigned i = 0; i < mTData.size(); i++)
		{
			mButtons.emplace_back(std::make_pair(pyro::gui::ClickableGUIEntity(mWindow, buttonSize), sf::VertexArray(sf::Quads, 4)));
			mButtons[i].first.setPosition(buttonPos.x + totalButtonSizeX * (i + 0.5f), buttonPos.y);

			sf::RectangleShape& buttonShape(mButtons[i].first.getShape());
			buttonShape.setTexture(&textures.get(static_cast<K>(i)));
			if (mTData[i].iconRect.width == 0)
				buttonShape.setTextureRect(sf::IntRect(0, 0, buttonShape.getTexture()->getSize().x, buttonShape.getTexture()->getSize().y));
			else
				buttonShape.setTextureRect(mTData[i].iconRect);
			buttonShape.setOutlineThickness(thickness);
			buttonShape.setOutlineColor(sf::Color::Black);

			sf::Vector2f buttonPos(mButtons[i].first.getPosition());
			sf::Vector2f halfSize(buttonSize / 2.f);
			mButtons[i].second[0].position = buttonPos - halfSize;
			mButtons[i].second[1].position = sf::Vector2f(buttonPos.x + halfSize.x, buttonPos.y - halfSize.y);
			mButtons[i].second[2].position = buttonPos + halfSize;
			mButtons[i].second[3].position = sf::Vector2f(buttonPos.x - halfSize.x, buttonPos.y + halfSize.y);

			for (unsigned j = 0; j < 4; j++)
				mButtons[i].second[j].color = mButtonColors.first;
		}

		mButtonOverlay[0].position = mButtons.front().second[0].position + sf::Vector2f(-margin, -margin);
		mButtonOverlay[1].position = mButtons.back().second[1].position + sf::Vector2f(margin, -margin);
		mButtonOverlay[2].position = mButtons.back().second[2].position + sf::Vector2f(margin, margin);
		mButtonOverlay[3].position = mButtons.front().second[3].position + sf::Vector2f(-margin, margin);

		mButtonOverlayTexture.loadFromFile("Assets/Textures/UnitBackground.png");
		sf::Vector2f textureSize(mButtonOverlayTexture.getSize());

		mButtonOverlay[0].texCoords = sf::Vector2f(0.f, 0.f);
		mButtonOverlay[1].texCoords = sf::Vector2f(textureSize.x, 0.f);
		mButtonOverlay[2].texCoords = textureSize;
		mButtonOverlay[3].texCoords = sf::Vector2f(0.f, textureSize.y);

		mStatTooltip = StatTooltipPtr(new StatTooltip<T>(mTData, mWindow,
			sf::FloatRect(mButtonOverlay[0].position.x,
			              mButtonOverlay[1].position.y,
			              mButtonOverlay[2].position.x - mButtonOverlay[0].position.x,
			              mButtonOverlay[2].position.y - mButtonOverlay[0].position.y)));
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mButtonOverlay, &mButtonOverlayTexture);
		for (const auto& button : mButtons)
		{
			target.draw(button.second, states);
			target.draw(button.first, states);
		}

		target.draw(*mStatTooltip, states);
	}

	template <typename T, typename K>
	int SpawnButtons<T, K>::handleEvent(const sf::Event& event)
	{
		for (unsigned i = 0; i < mButtons.size(); i++)
			if (mButtons[i].first.clicked(event))
				return i;

		return -1;
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::update()
	{
		sf::Vector2f mousePos(static_cast<sf::Vector2f>(sf::Mouse::getPosition(mWindow)));

		if (mousePos.x > mButtonOverlay[0].position.x && mousePos.y > mButtonOverlay[0].position.y
		 && mousePos.x < mButtonOverlay[2].position.x && mousePos.y < mButtonOverlay[2].position.y)
			for (unsigned i = 0; i < mButtons.size(); i++)
				if (mButtons[i].first.hover())
				{
					mStatTooltip->update(i);
					return;
				}

		mStatTooltip->update(-1);
	}

	template <typename T, typename K>
	void SpawnButtons<T, K>::updateButtonOverlay(unsigned gold)
	{
		for (unsigned i = 0; i < mButtons.size(); i++)
		{
			const sf::Color& color(mTData[i].cost > gold ? mButtonColors.second : mButtonColors.first);
			for (unsigned j = 0; j < 4; j++)
				mButtons[i].second[j].color = color;
		}
	}

	template <typename T, typename K>
	const sf::RectangleShape& SpawnButtons<T, K>::getButtonShape(unsigned i)
	{
		return mButtons[i].first.getShape();
	}
}