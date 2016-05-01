#ifndef Unit_H_
#define Unit_H_

#include "Entity.h"
#include "GlobalStructs.h"
#include "Animation.h"

#include <PYRO/ResourceHolder.h>
#include<PYRO/Audio/SoundPlayer.h>

class Unit : public Entity
{
	friend class BaseAI;
public:
	enum Type { Mage, Knight, Destroyer, TypeCount };
	enum class SoundID { MageAttack };
private:
	Type						mType;
	const float					mAttackRange;
	gStruct::Resource<sf::Time> mAttackRate;
	const float					mSpeed;
	const unsigned short		mReward;
								
	bool						mMoving;
	Animation					mWalkingAnimation;
protected:
	const unsigned short		mDamage;
	pyro::SoundPlayer<SoundID>& mSoundPlayer;

	bool						mAttacking;
	Animation					mAttackAnimation;

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