#ifndef UpgradeButtons_H_
#define UpgradeButtons_H_

#include "../UnitTurretData.h"

#include <PYRO/GUI/Button.h>

namespace gui
{
	template <typename T>
	class UpgradeButtons : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
	{
	private:
		using Buttons = std::vector<std::pair<pyro::gui::Button, sf::VertexArray>>;
		using TData = std::vector<T>;
	private:
		Buttons      mButtons;
		const TData& mTData;

	private:
		void setup(const sf::Vector2f& spawnButtonSize);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		UpgradeButtons(const TData& data, const sf::Vector2f& spawnButtonSize);
	};
}
#include "UpgradeButtons.inl"
#endif