#ifndef Base_H_
#define Base_H_

#include "Mage.h"
#include "Turret.h"
#include "GUI/SpawnBar.h"

#include <PYRO/Text.h>

#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Event.hpp>

#include <list>
#include <memory>

class Base : public Entity
{
private:
	pyro::SoundPlayer<Unit::SoundID>&	   mSoundPlayer;
	gui::SpawnBar						   mSpawnBar;

	sf::Int16							   mUnitTypeToSpawn;
protected:
	const pyro::TextureHolder<Unit::Type>& mTextureHolder;
	std::vector<gStruct::UnitData>&		   mUnitData;					   
	std::list<std::shared_ptr<Unit>>	   mUnits;
	std::vector<Turret>                    mTurrets;
	unsigned							   mGold;
								   		   

protected:
	void spawnUnit();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
	void handleUnitSpawn(Unit::Type type);
public:
	Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		 const pyro::TextureHolder<Unit::Type>& textures,
		 std::vector<gStruct::UnitData>& data,
		 pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
	virtual ~Base();
public:
	void attack(std::shared_ptr<Unit>& otherUnit);
	void attack(Base* otherBase);
	virtual void update(sf::Time dt);
	virtual void modifyGold(int amount);

	inline std::shared_ptr<Unit>& getFirstUnit() { return mUnits.front(); }
	inline bool hasUnits() const { return !mUnits.empty(); }

	friend sf::Packet& operator<<(sf::Packet& packet, Base& base);
	friend sf::Packet& operator>>(sf::Packet& packet, Base& base);
};
#endif