#ifndef GameState_H_
#define GameState_H_

#include "BaseAI.h"
#include "GlobalStructs.h"

#include <PYRO/State.h>
#include <PYRO/MusicPlayer.h>

class GameState : public pyro::State
{
private:
	enum class MusicID { Soundtrack };
private:
	pyro::TextureHolder<Unit::Type>  mUnitTextures;
	sf::Texture						 mBaseTexture;
									 
	std::vector<gStruct::UnitData>	 mUnitData;
	std::unique_ptr<Base>		     mLeftBase;
	std::unique_ptr<BaseAI>		     mRightBase;
								   	 
	sf::VertexArray				     mBackground;
	sf::Texture					     mBackgroundTexture;
									 
	pyro::MusicPlayer<MusicID>		 mMusicPlayer;
	pyro::SoundPlayer<Unit::SoundID> mSoundPlayer;

private:
	void setupBackground();
	virtual void setupResources();
public:
	GameState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	virtual bool handleEvent(const sf::Event& event);
	virtual bool update(sf::Time dt);
	virtual void draw();
};
#endif