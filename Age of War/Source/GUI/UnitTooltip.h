#ifndef UnitTooltip_H_
#define UnitTooltip_H_

#include "../GlobalStructs.h"

#include <PYRO/Text.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace gui
{
	class UnitTooltip : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		using UnitStats = std::pair<std::vector<pyro::Text>, pyro::Text>;
		using UnitData = std::vector<gStruct::UnitData>;
	private:
		sf::RectangleShape mShape;

		sf::Font           mFont;
		pyro::Text         mUnitTitle;
		pyro::Text         mUnitCost;
		UnitStats          mUnitStats;

		const UnitData&    mUnitData;
		sf::RenderWindow&  mWindow;

	private:
		void setup(sf::FloatRect overlayRect);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		UnitTooltip(const UnitData& unitData, sf::RenderWindow& window,
			        sf::FloatRect overlayRect);
	public:
		void update(int unitType);
	};
}
#endif