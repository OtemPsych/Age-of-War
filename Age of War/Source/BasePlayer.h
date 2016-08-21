#ifndef BasePlayer_H_
#define BasePlayer_H_

#include "Base.h"
#include "Camera.h"
#include "GUI/SpawnButtons.h"
#include "GUI/TurretPlacementIndicators.h"
#include "GUI/UpgradeButtons.h"

class BasePlayer : public Base
{
private:
	using UnitSpawnButtons   = gui::SpawnButtons<data::UnitData, Unit::UnitType>;
	using TurretSpawnButtons = gui::SpawnButtons<data::TurretData, Turret::TurretType>;

	using UnitUpgradeButtons   = gui::UpgradeButtons<data::UnitData, Unit::UnitType>;
	using TurretUpgradeButtons = gui::UpgradeButtons<data::TurretData, Turret::TurretType>;

public:
	BasePlayer(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
		       const pyro::TextureHolder<Unit::UnitType>& unit_textures,
		       const pyro::TextureHolder<Turret::TurretType>& turret_textures,
		       Side side, sf::RenderWindow* window, data::BaseData* base_data,
		       pyro::SoundPlayer<Unit::SoundID>* sound_player,
			   const std::vector<pyro::SceneNode*>* scene_layers);
public:
	virtual void modifyGold(int amount) override;
private:
	void init(const sf::FloatRect& world_bounds);
	void updateGoldGUI();
	virtual void handleEventCurrent(const sf::Event& event) override;
	virtual void updateCurrent(sf::Time dt) override;

private:
	sf::RenderWindow*               window_;
								    
	pyro::SpriteNode*               coin_;
	pyro::Text*                     coin_text_;
	sf::Font                        coin_font_;
	sf::Texture                     coin_texture_;
	data::AnimationData             coin_rotation_animation_data_;
	Animation*                      coin_rotation_animation_;
						            
	UnitSpawnButtons*               unit_buttons_;
	TurretSpawnButtons*             turret_buttons_;

	UnitUpgradeButtons*             unit_upgrade_buttons_;
	TurretUpgradeButtons*           turret_upgrade_buttons_;
						            
	bool                            active_turret_placement_indicators_;
						            
	pyro::VertexArrayNode*          turret_indicator_;
	gui::TurretPlacementIndicators* turret_placement_indicators_;
};									 

//class BasePlayer : public Base
//{
//private:
//	sf::Sprite		                    mCoinSprite;
//	std::unique_ptr<sf::RectangleShape> mTurretIndicator;
//	gui::TurretPlacementIndicators      mTurretPlacementIndicators;
//private:
//	void setupGoldGUI();
//	void updateGoldGUI();
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
//public:
//	void updateGUIPositions();
//};
#endif