#include "UnitButtons.h"

namespace gui
{
	UnitButtons::UnitButtons(sf::RenderWindow& window, const pyro::TextureHolder<Unit::Type>& textures,
		                     const std::vector<gStruct::UnitData>& unitData)
		: mButtonColors(std::make_pair(sf::Color(0, 128, 255), sf::Color(140, 140, 140)))
		, mButtonOverlay(sf::Quads, 4)
		, mUnitData(unitData)
	{
		setup(window, textures);
	}

	void UnitButtons::setup(sf::RenderWindow& window, const pyro::TextureHolder<Unit::Type>& textures)
	{
		const sf::Vector2f size(55.f, 60.f);
		const float thickness = 3.f;
		const float margin = thickness + 12.f;
		const float totalSizeX = size.x + margin;

		const sf::Vector2f buttonPos(window.getSize().x / 2.f - mUnitData.size() / 2.f * totalSizeX,
			                         size.y / 2.f + margin);

		for (unsigned i = 0; i < mUnitData.size(); i++)
		{
			mButtons.emplace_back(std::make_pair(pyro::gui::ClickableGUIEntity(window, size), sf::VertexArray(sf::Quads, 4)));
			mButtons[i].first.setPosition(buttonPos.x + totalSizeX * (i + 0.5f), buttonPos.y);
			
			sf::RectangleShape& shape(mButtons[i].first.getShape());
			shape.setTexture(&textures.get(static_cast<Unit::Type>(i)));
			shape.setTextureRect(mUnitData[i].walkRects[0]);
			shape.setOutlineThickness(thickness);
			shape.setOutlineColor(sf::Color::Black);

			sf::Vector2f buttonPos(mButtons[i].first.getPosition());
			sf::Vector2f halfSize(size / 2.f);
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
	}

	void UnitButtons::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mButtonOverlay, &mButtonOverlayTexture);
		for (const auto& button : mButtons) 
		{
			target.draw(button.second, states);
			target.draw(button.first, states);
		}
	}

	int UnitButtons::handleEvent(const sf::Event& event)
	{
		for (unsigned i = 0; i < mButtons.size(); i++)
			if (mButtons[i].first.clicked(event))
				return i;

		return -1;
	}

	void UnitButtons::update(unsigned gold)
	{
		for (unsigned i = 0; i < mButtons.size(); i++)
			if (mUnitData[i].cost > gold)
				for (unsigned j = 0; j < 4; j++)
					mButtons[i].second[j].color = mButtonColors.second;
			else
				for (unsigned j = 0; j < 4; j++)
					mButtons[i].second[j].color = mButtonColors.first;
	}
}