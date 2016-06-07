#ifndef SpawnButtons_H_
#define SpawnButtons_H_

#include "StatTooltip.h"
#include "../Tools/StringFunctions.h"

#include <PYRO/GUI/ClickableGUIEntity.h>

namespace gui
{
	template <typename T, typename K>
	class SpawnButtons : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		using Buttons = std::vector<std::pair<pyro::gui::ClickableGUIEntity, sf::VertexArray>>;
		using ButtonColors = std::pair<sf::Color, sf::Color>;
		using TData = std::vector<T>;
		using StatTooltipPtr = std::unique_ptr<StatTooltip<T>>;
	private:
		Buttons           mButtons;
		ButtonColors      mButtonColors;
		sf::VertexArray   mButtonOverlay;
		sf::Texture       mButtonOverlayTexture;

		StatTooltipPtr    mStatTooltip;

		const TData&      mTData;
		sf::RenderWindow& mWindow;

	private:
		void setup(const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		SpawnButtons(const TData& data, sf::RenderWindow& window,
			         const pyro::TextureHolder<K>& textures, sf::Vector2f buttonSize);
	public:
		int handleEvent(const sf::Event& event);
		void update();
		void updateButtonOverlay(unsigned gold);

		const sf::RectangleShape& getButtonShape(unsigned i);
	};
}
#include "SpawnButtons.inl"
#endif