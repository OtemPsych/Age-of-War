#ifndef UnitButtons_H_
#define UnitButtons_H_

#include "../Unit.h"
#include "../GlobalStructs.h"

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

		const UnitData& mUnitData;

	private:
		void setup(sf::RenderWindow& window, const pyro::TextureHolder<Unit::Type>& textures);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	public:
		UnitButtons(sf::RenderWindow& window, const pyro::TextureHolder<Unit::Type>& textures,
			        const std::vector<gStruct::UnitData>& unitData);
	public:
		int handleEvent(const sf::Event& event);
		void update(unsigned gold);
	};
}
#endif