#ifndef BasePlayer_H_
#define BasePlayer_H_

#include "Base.h"
#include "GUI/UnitButtons.h"

#include <PYRO/Text.h>
#include <PYRO/GUI/ClickableGUIEntity.h>
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Event.hpp>

class BasePlayer : public Base
{
private:
	sf::Font         mFont;
	pyro::Text		 mGoldText;
	sf::Texture		 mGoldCoinTexture;
	sf::Sprite		 mGoldCoinSprite;
					
	gui::UnitButtons mUnitButtons;

private:
	void setupGoldGUI();

	void updateGoldGUI();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds,
			   const sf::Texture& baseTexture, const pyro::TextureHolder<Unit::Type>& textures,
		       std::vector<gStruct::UnitData>& data,
		       pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	void handleEvent(const sf::Event& event);
	virtual void modifyGold(int amount);
};
#endif