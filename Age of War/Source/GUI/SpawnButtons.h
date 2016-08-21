#ifndef SpawnButtons_H_
#define SpawnButtons_H_

#include "StatTooltip.h"
#include "../Tools/StringFunctions.h"

#include <PYRO/GUI/Button.h>

namespace gui
{
	template <typename T, typename K>
	class SpawnButtons : public pyro::VertexArrayNode, private sf::NonCopyable
	{
	private:
		using Button       = std::pair<pyro::gui::Button*, pyro::VertexArrayNode*>;
		using ButtonColors = std::pair<sf::Color, sf::Color>;
		using TData        = std::vector<std::unique_ptr<T>>;

	public:
		SpawnButtons(const TData& Tdata, const pyro::TextureHolder<K>& textures,
			         const sf::Vector2f& button_size, sf::RenderWindow* window);
	public:
		int getTypeClicked();
		std::pair<std::vector<sf::Vector2f>, sf::Vector2f> getButtonProperties() const;
		std::unique_ptr<pyro::VertexArrayNode> getButtonBox(unsigned indicator);

		void updateButtonOverlay(unsigned gold);
		virtual void updateCurrent(sf::Time dt) override;
		virtual void handleEventCurrent(const sf::Event& event);
	private:
		void init(const pyro::TextureHolder<K>& textures, const sf::Vector2f& button_size);

	private:
		const TData&                  Tdata_;
		sf::RenderWindow*             window_;

		sf::Texture                   overlay_texture_;
		StatTooltip*                  stat_tooltip_;
		std::vector<Button>           buttons_;
		ButtonColors                  button_colors_;
		int                           Ttype_;
	};

	//template <typename T, typename K>
	//class SpawnButtons : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	//{
	//private:
	//	using StatTooltipPtr = std::unique_ptr<StatTooltip>;
	//private:
	//	sf::VertexArray   mButtonOutlines;
	//	sf::VertexArray   mButtonOverlay;

	//private:
	//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	//public:
	//	std::vector<sf::Vector2f> getButtonPositions() const;
	//	std::unique_ptr<pyro::VertexArrayNode> getButtonBox(unsigned i);
	//};
}
#include "SpawnButtons.inl"
#endif