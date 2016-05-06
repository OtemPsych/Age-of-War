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
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		       const pyro::TextureHolder<Unit::UnitType>& unitTextures,
		       std::vector<gStruct::UnitData>& unitData,
			   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
			   std::vector<gStruct::TurretData>& turretData,
		       pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	void handleEvent(const sf::Event& event);
	virtual void update(sf::Time dt) override;
	virtual void modifyGold(int amount) override;
};
#endif