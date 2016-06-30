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
		using namespace pyro::utils;

		setOriginFlags(mShape, OriginFlags::CenterX | OriginFlags::Top);
		mShape.setPosition(overlayRect.left + overlayRect.width / 2.f,
			               overlayRect.top + overlayRect.height + 10.f);
		mShape.setFillColor(sf::Color(0, 0, 0, 200));
		mShape.setOutlineThickness(2.f);
		mShape.setOutlineColor(sf::Color(153, 153, 153));

		mFont.loadFromFile("Assets/Fonts/Menu.ttf");

		mTTitle.setOriginFlags(OriginFlags::Left | OriginFlags::Top);
		mTTitle.setFont(mFont);
		mTTitle.setCharacterSize(20);
		mTTitle.setStyle(sf::Text::Bold);
		mTTitle.setTextColor(sf::Color(0, 204, 255));

		mTCost.setOriginFlags(OriginFlags::Right | OriginFlags::Top);
		mTCost.setFont(mFont);
		mTCost.setCharacterSize(18);
		mTCost.setStyle(sf::Text::Bold);
		mTCost.setTextColor(sf::Color(255, 255, 77));

		for (unsigned i = 0; i < 5; i++) {
			mTStats.emplace_back();
			mTStats.back().second.setOriginFlags(OriginFlags::Left | OriginFlags::CenterY);
			mTStats.back().second.setFont(mFont);
			mTStats.back().second.setCharacterSize(14);
			mTStats.back().first.setOriginFlags(OriginFlags::Right | OriginFlags::CenterY);
			mTStats.back().first.setFont(mFont);
			mTStats.back().first.setCharacterSize(14);
			mTStats.back().first.setTextColor(sf::Color(102, 255, 102));
		}

		mTStats[0].second.setString("Health");
		mTStats[1].second.setString("Damage");
		mTStats[2].second.setString("Range");
		mTStats[3].second.setString("Rate");
		mTStats[4].second.setString("Spawn Time");
	}

	template <typename T>
	void StatTooltip<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!mTTitle.getString().isEmpty()) {
			target.draw(mShape, states.transform *= getTransform());
			target.draw(mTTitle, states.transform *= mShape.getTransform());
			target.draw(mTCost, states);
			for (const auto& text : mTStats) {
				target.draw(text.first, states);
				target.draw(text.second, states);
			}
		}
	}

	template <typename T>
	void StatTooltip<T>::update(int TType)
	{
		if (TType != -1) {
			if (mTData[TType].name != mTTitle.getString()) {
				const auto& data = mTData[TType];
				const float padding = 6.f;

				// TTitle
				mTTitle.setString(data.name);
				mTTitle.setPosition(padding, padding);

				// TCost
				mTCost.setString(std::to_string(data.cost) + "g");
				mTCost.setPosition(mShape.getSize().x - padding, padding);

				// TStats
						// Setup Texts
				mTStats[0].first.setString(data.health == 0 ? "/" : std::to_string(data.health));
				mTStats[1].first.setString(std::to_string(data.damage));
				mTStats[2].first.setString(std::to_string(static_cast<int>(data.range)));
				mTStats[3].first.setString(strFunc::getPrecisionString(data.rate.asSeconds(), 1) + "s");
				mTStats[4].first.setString(strFunc::getPrecisionString(data.spawn.asSeconds(), 1) + "s");

						// Find Largest Width
				float largestWidth = mTStats.front().first.getGlobalBounds().width;
				for (unsigned i = 1; i < mTStats.size(); i++) {
					float newWidth = mTStats[i].first.getGlobalBounds().width;
					if (newWidth > largestWidth)
						largestWidth = newWidth;
				}

						// Set Text Positions
				float previousHeight = 25.f;
				for (auto& text : mTStats) {
					float textHeight = text.first.getGlobalBounds().height;
					text.first.setPosition(largestWidth + padding, textHeight / 2.f + previousHeight + padding);
					text.second.setPosition(largestWidth + padding + 6.f, textHeight / 2.f + previousHeight + padding);
					previousHeight = text.first.getPosition().y + textHeight / 2.f;
				}
			}
		}
		else
			mTTitle.setString("");
	}
}