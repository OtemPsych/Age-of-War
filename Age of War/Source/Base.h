#ifndef Base_H_
#define Base_H_

#include "Mage.h"
#include "GUI/SpawnBar.h"

#include <PYRO/Text.h>

#include <SFML/Window/Event.hpp>

#include <list>
#include <queue>

class Base : public Entity
{
private:
	const pyro::TextureHolder<Unit::Type>& mTextureHolder;
	pyro::SoundPlayer<Unit::SoundID>&	   mSoundPlayer;

	pyro::Text							   mGoldText;
	sf::Font							   mGoldFont;
	sf::Texture							   mGoldCoinTexture;
	sf::Sprite							   mGoldCoinSprite;

	gui::SpawnBar						   mSpawnBar;
protected:
	std::vector<Unit::UnitData>&		   mUnitData;					   
	std::list<std::unique_ptr<Unit>>	   mUnits;

	unsigned							   mGold;
								   		   

private:
	void updateGoldGUI();
	void spawnUnit();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
protected:
	void handleUnitSpawn(Unit::Type type);
public:
	Base(Side side, sf::IntRect worldBounds, const sf::Texture& baseTexture,
		 const pyro::TextureHolder<Unit::Type>& textures,
		 std::vector<Unit::UnitData>& data,
		 pyro::SoundPlayer<Unit::SoundID>& soundPlayer);
	virtual ~Base();
public:
	void attack(std::unique_ptr<Unit>& otherUnit);
	void attack(Base* otherBase);
	void handleEvent(const sf::Event& event);
	virtual void update(sf::Time dt);

	inline std::unique_ptr<Unit>& getFirstUnit() { return mUnits.front(); }
	inline bool hasUnits() const { return !mUnits.empty(); }
};
#endif