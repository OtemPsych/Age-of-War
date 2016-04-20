#ifndef Unit_H_
#define Unit_H_

#include "Entity.h"
#include "Animation.h"

#include <PYRO/ResourceTypedefs.h>
#include <PYRO/SoundPlayer.h>

class Unit : public Entity
{
	friend class BaseAI;
public:
	enum Type { Mage, Knight, Destroyer, TypeCount };
	enum class SoundID { MageAttack };
private:
	struct AttackRate
	{
		const sf::Time original;
		sf::Time	   current;

		explicit AttackRate(sf::Time rate);
	};
public:
	struct UnitData
	{
		Unit::Type				 type;
		unsigned				 health;
		unsigned				 damage;
		float					 range;
		sf::Time				 rate;
		float					 speed;
		sf::Time				 spawn;
		unsigned				 cost;
		float					 scale;
		std::vector<sf::IntRect> walkRects;
		std::vector<sf::IntRect> attackRects;
	};
protected:
	Type						mType;
	const unsigned				mDamage;
	const float					mAttackRange;
	AttackRate					mAttackRate;
	const float					mSpeed;
	const unsigned				mReward;
								
	bool						mMoving;
	bool						mAttacking;
								
	Animation					mWalkingAnimation;
	Animation					mAttackAnimation;

	pyro::SoundPlayer<SoundID>& mSoundPlayer;

protected:
	bool canAttackTarget(Entity& entity);
public:
	Unit(Side side, UnitData& data, const pyro::TextureHolder<Unit::Type>& textureHolder,
		 pyro::SoundPlayer<SoundID>& soundPlayer);
	virtual ~Unit();
public:
	virtual void attack(Entity& entity);
	virtual void update(sf::Time dt);

	inline void startMovement(bool state) { mMoving = state; }
	inline void stopAttacking() { mAttacking = false; }
	inline unsigned getRewardMoney() const { return mReward; }

	static std::vector<UnitData> initializeUnitData();
};
#endif