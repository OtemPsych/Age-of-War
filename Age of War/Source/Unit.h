#ifndef Unit_H_
#define Unit_H_

#include "Entity.h"
#include "GlobalStructs.h"
#include "Animation.h"

#include <PYRO/ResourceTypedefs.h>
#include <PYRO/SoundPlayer.h>

class Unit : public Entity
{
	friend class BaseAI;
public:
	enum Type { Mage, Knight, Destroyer, TypeCount };
	enum class SoundID { MageAttack };
protected:
	Type						mType;
	const unsigned short		mDamage;
	const float					mAttackRange;
	gStruct::Resource<sf::Time> mAttackRate;
	const float					mSpeed;
	const unsigned short		mReward;
								
	bool						mMoving;
	bool						mAttacking;
								
	Animation					mWalkingAnimation;
	Animation					mAttackAnimation;

	pyro::SoundPlayer<SoundID>& mSoundPlayer;

protected:
	bool canAttackTarget(Entity& entity);
public:
	Unit(Side side, gStruct::UnitData& data, const pyro::TextureHolder<Unit::Type>& textureHolder,
		 pyro::SoundPlayer<SoundID>& soundPlayer);
	virtual ~Unit();
public:
	virtual void attack(Entity& entity);
	virtual void update(sf::Time dt);

	inline void startMovement(bool state) { mMoving = state; }
	inline void stopAttacking() { mAttacking = false; }
	inline unsigned getRewardMoney() const { return mReward; }
};
#endif