#ifndef Base_H_
#define Base_H_

#include "Turret.h"
#include "GUI/SpawnBar.h"
#include "SmokeSystem.h"

#include <SFML/Network/Packet.hpp>

namespace data { struct BaseData; }
class Base : public HealthEntity, private sf::NonCopyable
{
public:
	enum BaseType { DefaultBase };
private:
	enum class PacketTypeID { NewUnit, NewTurret };

public:
	Base(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
		 const pyro::TextureHolder<Unit::UnitType>& unit_textures,
		 const pyro::TextureHolder<Turret::TurretType>& turret_textures,
		 Side side, data::BaseData* base_data,
		 pyro::SoundPlayer<Unit::SoundID>* sound_player,
		 const std::vector<pyro::SceneNode*>* scene_layers);
	virtual ~Base();
public:
	void attack(HealthEntity* enemy);
	virtual void modifyGold(int amount);

	inline Unit* getFirstUnit() { return !units_.empty() ? units_.front() : nullptr; }
	inline bool hasUnits() const { return !units_.empty(); }

	friend sf::Packet& operator<<(sf::Packet& packet, Base& base);
	friend sf::Packet& operator>>(sf::Packet& packet, Base& base);
protected:
	void handleTurretSpawn(Turret::TurretType type, int turret_indicator);
	void handleUnitSpawn(Unit::UnitType type);
	//void handleUnitUpgrade(Unit::UnitType type, Unit::UpgradeType upgrade_type);
	void spawnUnit();
	virtual void updateCurrent(sf::Time dt) override;

protected:
	const std::vector<pyro::SceneNode*>*                 scene_layers_;

	const pyro::TextureHolder<Unit::UnitType>&           unit_textures_;
	std::vector<std::unique_ptr<data::UnitData>>         unit_data_;
	std::vector<Unit*>                                   units_;
												         
	const pyro::TextureHolder<Turret::TurretType>&       turret_textures_;
	std::vector<std::unique_ptr<data::TurretData>>       turret_data_;
	std::vector<std::pair<Turret*, int>>                 turrets_;
	sf::Int16                                            turret_type_to_spawn_;
	std::array<sf::FloatRect, 3>                         turret_window_rects_;

	std::vector<std::unique_ptr<data::ValueDisplayData>> value_display_data_;

	unsigned                                             gold_;
private:							                     
	data::BaseData*                                      base_data_;
	pyro::SoundPlayer<Unit::SoundID>*                    sound_player_;
	const sf::Font&                                      damage_font_;
												         
	gui::SpawnBar*                                       spawn_bar_;
									                     
	sf::Int16                                            mp_unit_type_;
	std::pair<sf::Int16, sf::Int8>                       mp_turret_info_;
	unsigned                                             current_population_;
									                     
	SmokeSystem*                                         smoke_system_;
};

//class Base : public HealthEntity, private sf::NonCopyable
//{
//	gui::SpawnBar                                  mSpawnBar;
//protected:
//	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
//public:
//	void drawUnitDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const;
//};
#endif