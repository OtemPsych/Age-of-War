#ifndef GameState_H_
#define GameState_H_

#include "../BasePlayer.h"
#include "../BaseAI.h"
#include "../GlobalStructs.h"

#include <PYRO/State.h>
#include <PYRO/Audio/MusicPlayer.h>

class GameState : public pyro::State
{
protected:
	enum class MusicID { Soundtrack };
private:
	sf::Sprite			                    mBackground;
	sf::Texture					            mBackgroundTexture;

	sf::Sprite                              mCursor;
	sf::Texture                             mCursorTexture;
protected:							  	    
	pyro::TextureHolder<Unit::UnitType>     mUnitTextures;
	pyro::TextureHolder<Turret::TurretType> mTurretTextures;
	sf::Texture						        mBaseTexture;
									        
	std::vector<gStruct::UnitData>	        mUnitData;
	std::vector<gStruct::TurretData>        mTurretData;
	std::unique_ptr<BasePlayer>		        mBasePlayer;
	std::unique_ptr<Base>		            mBaseOpponent;
									        
	pyro::MusicPlayer<MusicID>		        mMusicPlayer;
	pyro::SoundPlayer<Unit::SoundID>        mSoundPlayer;

	sf::IntRect                             mWindowBounds;
	sf::IntRect                             mWorldBounds;

private:
	void setupBackground();
	virtual void setupResources() override;
public:
	GameState(pyro::StateStack& stack, sf::RenderWindow& window);
	virtual ~GameState();
public:
	void unpauseMusic();

	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
};
#endif