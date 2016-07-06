#ifndef Unit_H_
#define Unit_H_

#include "HealthEntity.h"
#include "Animation.h"
#include "ValueDisplays.h"

#include <PYRO/Audio/SoundPlayer.h>
#include <PYRO/ResourceHolder.h>

class Unit : public HealthEntity
{
public:
	enum GeneralUnitType { Melee, Ranged };
	enum UnitType { Mage, Knight, Samurai, Shadow, Destroyer, Executioner, TypeCount };
	enum class UnitUpgradeType { Health, Damage, Range, Rate, TypeCount};
	enum class SoundID { MageAttack, KnightAttack, TypeCount };
private:
	pyro::utils::Resource<sf::Time> mAttackRate;
	const float                     mSpeed;
								    
	bool                            mMoving;
	Animation                       mWalkingAnimation;
protected:						    
	GeneralUnitType                 mGeneralUnitType;
	UnitType                        mUnitType;
	const float                     mAttackRange;
	const unsigned short            mDamage;
	pyro::SoundPlayer<SoundID>&     mSoundPlayer;
								    
	ValueDisplays                   mDamageDisplays;
								    
	bool                            mAttacking;
	Animation                       mAttackAnimation;

protected:
	bool enemyInRange(HealthEntity& enemy);
	virtual void handleAttackAnimation(HealthEntity& enemy);
public:
	Unit(Side side, sf::Font& damageDisplayFont, data::UnitData& data,
		 const pyro::TextureHolder<UnitType>& unitTextures, pyro::SoundPlayer<SoundID>& soundPlayer);
	virtual ~Unit();
public:
	void drawDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void attack(HealthEntity& enemy);
	virtual void update(sf::Time dt) override;

	inline void startMovement(bool flag) { mMoving = flag; }
	inline void stopAttacking() { mAttacking = false; }
	inline GeneralUnitType getGeneralUnitType() const { return mGeneralUnitType; }
	inline UnitType        getUnitType() const { return mUnitType; }
};
#endif