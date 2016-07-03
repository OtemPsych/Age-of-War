#ifndef BasePlayer_H_
#define BasePlayer_H_

#include "Base.h"
#include "GUI/SpawnButtons.h"
#include "GUI/TurretPlacementIndicators.h"

class BasePlayer : public Base
{
private:
	using UnitSpawnButtons = gui::SpawnButtons<gStruct::UnitData, Unit::UnitType>;
	using TurretSpawnButtons = gui::SpawnButtons<gStruct::TurretData, Turret::TurretType>;
private:
	sf::Font                            mCoinFont;
	pyro::Text		                    mCoinText;
	sf::Texture		                    mCoinTexture;
	sf::Sprite		                    mCoinSprite;
	Animation                           mCoinRotateAnimation;
					                    
	UnitSpawnButtons                    mUnitButtons;
	TurretSpawnButtons                  mTurretButtons;

	bool                                mActiveTurretPlacementIndicators;
	std::unique_ptr<sf::RectangleShape> mTurretIndicator;
	gui::TurretPlacementIndicators      mTurretPlacementIndicators;

	sf::RenderWindow&                   mWindow;

private:
	void setupGoldGUI();

	void updateGoldGUI();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	BasePlayer(sf::RenderWindow& window, sf::IntRect worldBounds, sf::Font& font, const sf::Texture& baseTexture,
		       const pyro::TextureHolder<Unit::UnitType>& unitTextures,
			   const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		       pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
public:
	void updateGUIPositions();
	void handleEvent(const sf::Event& event);
	virtual void update(sf::Time dt) override;
	virtual void modifyGold(int amount) override;
};
#endif