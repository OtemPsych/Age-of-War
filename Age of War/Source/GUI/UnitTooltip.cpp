#include "UnitTooltip.h"
#include "../StringFunctions.h"

#include <SFML/Window/Mouse.hpp>

namespace gui
{
	UnitTooltip::UnitTooltip(const UnitData& unitData, sf::RenderWindow& window,
		                     sf::FloatRect overlayRect)
		: mUnitData(unitData)
		, mWindow(window)
		, mShape(sf::Vector2f(overlayRect.width, 110.f))
	{
		setup(overlayRect);
	}


	void UnitTooltip::setup(sf::FloatRect overlayRect)
	{
		sf::FloatRect shapeLBounds(mShape.getLocalBounds());
		mShape.setOrigin(shapeLBounds.width / 2.f, shapeLBounds.height / 2.f);
		sf::FloatRect shapeGBounds(mShape.getGlobalBounds());
		mShape.setPosition(overlayRect.left + shapeGBounds.width / 2.f,
			               overlayRect.top + overlayRect.height + shapeGBounds.height / 2.f + 10.f);
		mShape.setFillColor(sf::Color(0, 0, 0, 200));
		mShape.setOutlineThickness(2.f);
		mShape.setOutlineColor(sf::Color(153, 153, 153));

		mFont.loadFromFile("Assets/Fonts/Menu.ttf");

		mUnitTitle.setFont(mFont);
		mUnitTitle.setCharacterSize(20);
		mUnitTitle.setStyle(sf::Text::Bold);
		mUnitTitle.setColor(sf::Color(0, 204, 255));

		mUnitCost.setFont(mFont);
		mUnitCost.setCharacterSize(18);
		mUnitCost.setStyle(sf::Text::Bold);
		mUnitCost.setColor(sf::Color(255, 255, 77));

		mUnitStats.second.setFont(mFont);
		mUnitStats.second.setCharacterSize(14);
		for (unsigned i = 0; i < 5; i++)
		{
			mUnitStats.first.emplace_back(pyro::Text());
			mUnitStats.first.back().setFont(mFont);
			mUnitStats.first.back().setCharacterSize(14);
			mUnitStats.first.back().setColor(sf::Color(102, 255, 102));
		}
	}

	void UnitTooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!mUnitTitle.getString().isEmpty())
		{
			states.transform *= getTransform();
			target.draw(mShape, states);
			target.draw(mUnitTitle, states.transform *= mShape.getTransform());
			target.draw(mUnitCost, states);
			target.draw(mUnitStats.second, states);
			for (const auto& text : mUnitStats.first)
				target.draw(text, states);
		}
	}

	void UnitTooltip::update(int unitType)
	{
		if (unitType != -1)
		{
			if (mUnitData[unitType].name != mUnitTitle.getString())
			{
				const auto& data = mUnitData[unitType];
				const float padding = 6.f;

				// Unit Title
				mUnitTitle.setString(data.name);
				sf::FloatRect titleGBounds(mUnitTitle.getGlobalBounds());
				mUnitTitle.setPosition(titleGBounds.width / 2.f + padding, titleGBounds.height / 2.f + padding);

				// Unit Cost
				mUnitCost.setString(std::to_string(data.cost) + "g");
				sf::FloatRect costGBounds(mUnitCost.getGlobalBounds());
				mUnitCost.setPosition(mShape.getSize().x - costGBounds.width / 2.f - padding, costGBounds.height / 2.f + padding);

				// Unit Stats
					// First
						// Setup Texts
				mUnitStats.first[0].setString(std::to_string(data.health));
				mUnitStats.first[1].setString(std::to_string(data.damage));
				mUnitStats.first[2].setString(std::to_string(static_cast<int>(data.range)));
				mUnitStats.first[3].setString(strFunc::getPrecisionString(data.rate.asSeconds(), 1) + "s");
				mUnitStats.first[4].setString(strFunc::getPrecisionString(data.spawn.asSeconds(), 1) + "s");

						// Find Largest Width
				float largestWidth = mUnitStats.first.front().getGlobalBounds().width;
				for (unsigned i = 1; i < mUnitStats.first.size(); i++)
				{
					float newWidth = mUnitStats.first[i].getGlobalBounds().width;
					if (newWidth > largestWidth)
						largestWidth = newWidth;
				}

						// Set Text Positions
				float previousHeight = mUnitTitle.getPosition().y + titleGBounds.height / 2.f;
				for (auto& text : mUnitStats.first)
				{
					sf::FloatRect textGBounds(text.getGlobalBounds());
					text.setPosition(largestWidth - textGBounds.width / 2.f + padding,
									 textGBounds.height / 2.f + previousHeight + padding);

					previousHeight = text.getPosition().y + textGBounds.height / 2.f;
				}

					// Second
				mUnitStats.second.setString("Health\nDamage\nRange\nAttack Rate\nSpawnTime");
				sf::FloatRect descGBounds(mUnitStats.second.getGlobalBounds());
				mUnitStats.second.setPosition(largestWidth + descGBounds.width / 2.f + padding * 2,
					mUnitStats.first.front().getPosition().y + descGBounds.height / 2.f - mUnitStats.first.front().getGlobalBounds().height / 2.f);
			}
		}
		else
			mUnitTitle.setString("");
	}
}