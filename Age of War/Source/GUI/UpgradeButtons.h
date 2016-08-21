#ifndef UpgradeButtons_H_
#define UpgradeButtons_H_

#include <PYRO/GUI/Button.h>

namespace gui
{
	template <typename T, typename K>
	class UpgradeButtons : public pyro::SceneNode, private sf::NonCopyable
	{
	private:
		struct Button : public pyro::gui::Button {
			pyro::VertexArrayNode*                overlay;
			data::UnitTurretData::UpgradeStatType stat_type;

			Button(const sf::Vector2f& button_size, sf::RenderWindow* window);
		};
	private:
		using ButtonColors = std::pair<sf::Color, sf::Color>;
		using TData = std::vector<std::unique_ptr<T>>;
		using UpgradeButtonTextureHolder = pyro::TextureHolder<data::UnitTurretData::UpgradeStatType>;

	public:
		UpgradeButtons(const TData& data, const std::pair<std::vector<sf::Vector2f>, sf::Vector2f>& button_properties,
					   sf::RenderWindow* window);
		//UpgradeButtons(const TData& data, const sf::Vector2f& spawnButtonSize);
	private:
		void setup(const std::pair<std::vector<sf::Vector2f>, sf::Vector2f>& button_properties);
		void setupResources();

	private:
		const TData&                       data_;
		sf::RenderWindow*                  window_;

		UpgradeButtonTextureHolder         button_textures_;
		std::vector<std::pair<Button*, K>> buttons_;
		ButtonColors                       button_colors_;
		//Buttons      mButtons;
		//const TData& mTData;
	};
}
#include "UpgradeButtons.inl"
#endif