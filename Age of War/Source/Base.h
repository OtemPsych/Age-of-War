#ifndef Base_H_
#define Base_H_

#include "Turret.h"
#include "GUI/SpawnBar.h"

#include <SFML/Network/Packet.hpp>

class Base : public HealthEntity, private sf::NonCopyable
{
private:
	pyro::SoundPlayer<Unit::SoundID>&              mSoundPlayer;
	gui::SpawnBar                                  mSpawnBar;
									               
	sf::Int16                                      mUnitTypeToSpawn;
protected:									       
	const pyro::TextureHolder<Unit::UnitType>&     mUnitTextures;
	std::vector<gStruct::UnitData>&                mUnitData;
	std::vector<std::unique_ptr<Unit>>             mUnits;

	const pyro::TextureHolder<Turret::TurretType>& mTurretTextures;
	std::vector<gStruct::TurretData>&              mTurretData;
	std::vector<Turret>                            mTurrets;
											       
	unsigned short                                 mGold;

protected:
	void handleUnitSpawn(Unit::UnitType type);
	void spawnUnit();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		 const pyro::TextureHolder<Unit::UnitType>& unitTextures,
		 std::vector<gStruct::UnitData>& unitData,
		 const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		 std::vector<gStruct::TurretData>& turretData,
		 pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
	virtual ~Base();
public:
	void attack(Unit& enemyUnit);
	void attack(Base& enemyBase);
	virtual void update(sf::Time dt) override;
	virtual void modifyGold(int amount);

	inline Unit& getFirstUnit() { return *mUnits.front(); }
	inline bool hasUnits() const { return !mUnits.empty(); }

	friend sf::Packet& operator<<(sf::Packet& packet, Base& base);
	friend sf::Packet& operator>>(sf::Packet& packet, Base& base);
};
#endif