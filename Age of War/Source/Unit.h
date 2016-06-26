#ifndef Unit_H_
#define Unit_H_

#include "AttackEntity.h"
#include "Animation.h"

#include <PYRO/Audio/SoundPlayer.h>
#include <PYRO/ResourceHolder.h>

class Unit : public HealthEntity, public AttackEntity
{
public:
	enum GeneralUnitType { Melee, Ranged };
	enum UnitType { Mage, Knight, Samurai, Shadow, Destroyer, Executioner, TypeCount };
	enum class SoundID { MageAttack, KnightAttack, TypeCount };
private:
	const float                 mSpeed;
	bool                        mMoving;
	Animation                   mWalkingAnimation;
protected:
	GeneralUnitType             mGeneralUnitType;
	UnitType                    mUnitType;
	pyro::SoundPlayer<SoundID>& mSoundPlayer;

	Animation                   mAttackAnimation;

protected:
	bool enemyInRange(HealthEntity& enemy);
	virtual void handleAttackAnimation(HealthEntity& enemy);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Unit(Side side, sf::Font& font, gStruct::UnitData& data,
		 const pyro::TextureHolder<UnitType>& unitTextures, pyro::SoundPlayer<SoundID>& soundPlayer);
	virtual ~Unit();
public:
	virtual void attack(HealthEntity& enemy);
	virtual void update(sf::Time dt) override;

	inline void startMovement(bool flag) { mMoving = flag; }
	inline GeneralUnitType getGeneralUnitType() const { return mGeneralUnitType; }
	inline UnitType        getUnitType() const { return mUnitType; }
};
#endif