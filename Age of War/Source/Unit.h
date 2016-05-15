#ifndef Unit_H_
#define Unit_H_

#include "HealthEntity.h"
#include "Animation.h"

#include <PYRO/Audio/SoundPlayer.h>
#include <PYRO/ResourceHolder.h>

class Unit : public HealthEntity
{
public:
	enum GeneralUnitType { Melee, Ranged };
	enum UnitType { Mage, Knight, Destroyer, Executioner, Shadow, Samurai, TypeCount };
	enum class SoundID { MageAttack, KnightAttack, TypeCount };
private:
	const float                 mAttackRange;
	gStruct::Resource<sf::Time> mAttackRate;
	const float                 mSpeed;
	const unsigned short        mGoldReward;

	bool                        mMoving;
	Animation                   mWalkingAnimation;
protected:
	GeneralUnitType             mGeneralUnitType;
	UnitType                    mUnitType;
	const unsigned short        mDamage;
	pyro::SoundPlayer<SoundID>& mSoundPlayer;

	bool                        mAttacking;
	Animation                   mAttackAnimation;

protected:
	bool canAttackTarget(HealthEntity& enemy);
public:
	Unit(Side side, gStruct::UnitData& data, const pyro::TextureHolder<UnitType>& unitTextures,
		 pyro::SoundPlayer<SoundID>& soundPlayer);
	virtual ~Unit();
public:
	virtual void attack(HealthEntity& enemy);
	virtual void update(sf::Time dt) override;

	inline void startMovement(bool flag) { mMoving = flag; }
	inline void stopAttacking() { mAttacking = false; }
	inline unsigned getGoldReward() const { return mGoldReward; }
};
#endif