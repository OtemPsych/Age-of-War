#ifndef MenuState_H_
#define MenuState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/ClickableGUIEntity.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

class MenuState : public pyro::State
{
private:
	enum ButtonType { Play, Multiplayer, Quit, TypeCount };
private:
	using Buttons = std::vector<pyro::gui::ClickableGUIEntity>;
private:
	sf::Sprite  mBackgroundSprite;
	sf::Texture mBackgroundTexture;

	sf::Font    mFont;
	Buttons		mButtons;

public:
	MenuState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	virtual bool handleEvent(const sf::Event& event);
	virtual bool update(sf::Time dt);
	virtual void draw();
};
#endif