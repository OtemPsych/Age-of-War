#ifndef GameOverState_H_
#define GameOverState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/Button.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class GameOverState : public pyro::State
{
public:
	enum class GameOverType { None, Victory, Defeat };
private:
	GameOverType                  mGameOverType;
	sf::Sprite                    mCursor;
	sf::Texture                   mCursorTexture;

	sf::VertexArray               mBackgroundTint;
	sf::Sprite                    mSprite;
	sf::Texture                   mSpriteTexture;

	pyro::gui::Button mContinueButton;
	sf::Font                      mFont;

private:
	virtual void setupResources() override;
public:
	GameOverState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	void setGameOverType(GameOverType type);

	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
};
#endif