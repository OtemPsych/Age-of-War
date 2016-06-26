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
	enum UnitType { Mage, Knight, Samurai, Shadow, Destroyer, Executioner, TypeCount };
	enum class SoundID { MageAttack, KnightAttack, TypeCount };
protected:
	struct DamageDisplay {
		pyro::Text                      text;
		sf::Vector2f                    velocity;
		sf::Vector2f                    acceleration;
		pyro::utils::Resource<sf::Time> lifetime;

		DamageDisplay(sf::FloatRect enemyGBounds, sf::Font& font, unsigned short damage, bool ally);
		void operator=(const DamageDisplay& copy);
	};
private:
	const float                 mAttackRange;
	gStruct::Resource<sf::Time> mAttackRate;
	const float                 mSpeed;

	bool                        mMoving;
	Animation                   mWalkingAnimation;
protected:
	GeneralUnitType             mGeneralUnitType;
	UnitType                    mUnitType;
	const unsigned short        mDamage;
	pyro::SoundPlayer<SoundID>& mSoundPlayer;

	std::vector<DamageDisplay>  mDamageDisplays;
	sf::Font&                   mDamageDisplayFont;

	bool                        mAttacking;
	Animation                   mAttackAnimation;

protected:
	void updateDamageDisplays(sf::Time dt);

	bool enemyInRange(HealthEntity& enemy);
	virtual void handleAttackAnimation(HealthEntity& enemy);
public:
	Unit(Side side, sf::Font& font, gStruct::UnitData& data,
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