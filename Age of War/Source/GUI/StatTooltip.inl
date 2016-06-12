namespace gui
{
	template <typename T>
	StatTooltip<T>::StatTooltip(const TData& data, sf::RenderWindow& window, sf::FloatRect overlayRect)
		: mTData(data)
		, mWindow(window)
		, mShape(sf::Vector2f(200.f, 110.f))
	{
		setup(overlayRect);
	}

	template <typename T>
	void StatTooltip<T>::setup(sf::FloatRect overlayRect)
	{
		sf::FloatRect shapeLBounds(mShape.getLocalBounds());
		mShape.setOrigin(shapeLBounds.width / 2.f, shapeLBounds.height / 2.f);
		sf::FloatRect shapeGBounds(mShape.getGlobalBounds());
		mShape.setPosition(overlayRect.left + overlayRect.width / 2.f,
			               overlayRect.top + overlayRect.height + shapeGBounds.height / 2.f + 10.f);
		mShape.setFillColor(sf::Color(0, 0, 0, 200));
		mShape.setOutlineThickness(2.f);
		mShape.setOutlineColor(sf::Color(153, 153, 153));

		mFont.loadFromFile("Assets/Fonts/Menu.ttf");

		mTTitle.setOriginFlags(pyro::utils::OriginFlags::Center);
		mTTitle.setFont(mFont);
		mTTitle.setCharacterSize(20);
		mTTitle.setStyle(sf::Text::Bold);
		mTTitle.setTextColor(sf::Color(0, 204, 255));
		mTTitle.setShadowColor(sf::Color::Transparent);

		mTCost.setOriginFlags(mTTitle.getOriginFlags());
		mTCost.setFont(mFont);
		mTCost.setCharacterSize(18);
		mTCost.setStyle(sf::Text::Bold);
		mTCost.setTextColor(sf::Color(255, 255, 77));
		mTCost.setShadowColor(mTTitle.getShadowColor());

		mTStats.second.setOriginFlags(mTTitle.getOriginFlags());
		mTStats.second.setFont(mFont);
		mTStats.second.setCharacterSize(14);
		mTStats.second.setShadowColor(mTTitle.getShadowColor());
		for (unsigned i = 0; i < 5; i++) {
			mTStats.first.emplace_back(pyro::Text());
			mTStats.first.back().setOriginFlags(mTStats.second.getOriginFlags());
			mTStats.first.back().setFont(mFont);
			mTStats.first.back().setCharacterSize(14);
			mTStats.first.back().setTextColor(sf::Color(102, 255, 102));
			mTStats.first.back().setShadowColor(mTStats.second.getShadowColor());
		}
	}

	template <typename T>
	void StatTooltip<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!mTTitle.getString().isEmpty())
		{
			states.transform *= getTransform();
			target.draw(mShape, states);
			target.draw(mTTitle, states.transform *= mShape.getTransform());
			target.draw(mTCost, states);
			target.draw(mTStats.second, states);
			for (const auto& text : mTStats.first)
				target.draw(text, states);
		}
	}

	template <typename T>
	void StatTooltip<T>::update(int TType)
	{
		if (TType != -1)
		{
			if (mTData[TType].name != mTTitle.getString())
			{
				const auto& data = mTData[TType];
				const float padding = 6.f;

				// TTitle
				mTTitle.setString(data.name);
				sf::FloatRect titleGBounds(mTTitle.getGlobalBounds());
				mTTitle.setPosition(titleGBounds.width / 2.f + padding, titleGBounds.height / 2.f + padding);

				// TCost
				mTCost.setString(std::to_string(data.cost) + "g");
				sf::FloatRect costGBounds(mTCost.getGlobalBounds());
				mTCost.setPosition(mShape.getSize().x - costGBounds.width / 2.f - padding, costGBounds.height / 2.f + padding);

				// TStats
					// First
							// Setup Texts
				mTStats.first[0].setString(data.health == 0 ? "/" : std::to_string(data.health));
				mTStats.first[1].setString(std::to_string(data.damage));
				mTStats.first[2].setString(std::to_string(static_cast<int>(data.range)));
				mTStats.first[3].setString(strFunc::getPrecisionString(data.rate.asSeconds(), 1) + "s");
				mTStats.first[4].setString(strFunc::getPrecisionString(data.spawn.asSeconds(), 1) + "s");

							// Find Largest Width
				float largestWidth = mTStats.first.front().getGlobalBounds().width;
				for (unsigned i = 1; i < mTStats.first.size(); i++)
				{
					float newWidth = mTStats.first[i].getGlobalBounds().width;
					if (newWidth > largestWidth)
						largestWidth = newWidth;
				}

							// Set Text Positions
				float previousHeight = mTTitle.getPosition().y + titleGBounds.height / 2.f;
				for (auto& text : mTStats.first)
				{
					sf::FloatRect textGBounds(text.getGlobalBounds());
					text.setPosition(largestWidth - textGBounds.width / 2.f + padding,
						textGBounds.height / 2.f + previousHeight + padding);

					previousHeight = text.getPosition().y + textGBounds.height / 2.f;
				}

					// Second
				mTStats.second.setString("Health\nDamage\nRange\nAttack Rate\nSpawn Time");
				sf::FloatRect descGBounds(mTStats.second.getGlobalBounds());
				mTStats.second.setPosition(largestWidth + descGBounds.width / 2.f + padding * 2,
					mTStats.first.front().getPosition().y + descGBounds.height / 2.f - mTStats.first.front().getGlobalBounds().height / 2.f);
			}
		}
		else
			mTTitle.setString("");
	}
}