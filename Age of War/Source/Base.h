#ifndef Base_H_
#define Base_H_

#include "Turret.h"
#include "GUI/SpawnBar.h"
#include "SmokeSystem.h"

#include <SFML/Network/Packet.hpp>

class Base : public HealthEntity, private sf::NonCopyable
{
private:
	enum class PacketTypeID { NewUnit, NewTurret };
private:
	pyro::SoundPlayer<Unit::SoundID>&              mSoundPlayer;
	gui::SpawnBar                                  mSpawnBar;
									               
	sf::Int16                                      mMPUnitType;
	std::pair<sf::Int16, sf::Int8>                 mMPTurretInfo;

	SmokeSystem                                    mSmokeSystem;

	sf::Font&                                      mDisplayDamageFont;
protected:									       
	const pyro::TextureHolder<Unit::UnitType>&     mUnitTextures;
	std::vector<gStruct::UnitData>&                mUnitData;
	std::vector<std::shared_ptr<Unit>>             mUnits;

	const pyro::TextureHolder<Turret::TurretType>& mTurretTextures;
	std::vector<gStruct::TurretData>&              mTurretData;
	std::vector<std::pair<Turret, int>>            mTurrets;
	sf::Int16                                      mTurretTypeToSpawn;

	std::array<sf::FloatRect, 3>                   mTurretWindowRects;
											       
	unsigned short                                 mGold;

protected:
	void handleTurretSpawn(Turret::TurretType type, int turretIndicator);
	void handleUnitSpawn(Unit::UnitType type);
	void spawnUnit();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Base(Side side, sf::IntRect worldBounds, sf::Font& font, const sf::Texture& baseTexture,
		 const pyro::TextureHolder<Unit::UnitType>& unitTextures,
		 std::vector<gStruct::UnitData>& unitData,
		 const pyro::TextureHolder<Turret::TurretType>& turretTextures,
		 std::vector<gStruct::TurretData>& turretData,
		 pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
	virtual ~Base();
public:
	void drawUnitDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const;

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