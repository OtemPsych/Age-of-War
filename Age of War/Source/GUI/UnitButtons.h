#ifndef UnitButtons_H_
#define UnitButtons_H_

#include "../Unit.h"
#include "UnitTooltip.h"

#include <PYRO/GUI/ClickableGUIEntity.h>
#include <PYRO/ResourceHolder.h>

namespace gui
{
	class UnitButtons : public sf::Drawable, private sf::NonCopyable
	{
	private:
		using Buttons = std::vector<std::pair<pyro::gui::ClickableGUIEntity, sf::VertexArray>>;
		using ButtonColors = std::pair<sf::Color, sf::Color>;
		using UnitData = std::vector<gStruct::UnitData>;
	private:
		Buttons         mButtons;
		ButtonColors    mButtonColors;
		sf::VertexArray mButtonOverlay;
		sf::Texture     mButtonOverlayTexture;

		std::unique_ptr<UnitTooltip> mUnitTooltip;

		const UnitData& mUnitData;
		sf::RenderWindow& mWindow;

	private:
		void setup(sf::RenderWindow& window, const pyro::TextureHolder<Unit::UnitType>& textures);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		UnitButtons(sf::RenderWindow& window, const pyro::TextureHolder<Unit::UnitType>& textures,
			        const std::vector<gStruct::UnitData>& unitData);
	public:
		int handleEvent(const sf::Event& event);
		void update();
		void updateButtonOverlay(unsigned gold);
	};
}
#endif