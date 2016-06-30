#ifndef StatTooltip_H_
#define StatTooltip_H_

#include <PYRO/Text.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace gui
{
	template <typename T>
	class StatTooltip : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		using Stats = std::vector<std::pair<pyro::Text, pyro::Text>>;
		using TData = std::vector<T>;
	private:
		sf::RectangleShape mShape;

		sf::Font           mFont;
		pyro::Text         mTTitle;
		pyro::Text         mTCost;
		Stats              mTStats;

		const TData&       mTData;
		sf::RenderWindow&  mWindow;

	private:
		void setup(sf::FloatRect overlayRect);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		StatTooltip(const TData& data, sf::RenderWindow& window, sf::FloatRect overlayRect);
	public:
		void update(int TType);
	};
}
#include "StatTooltip.inl"
#endif