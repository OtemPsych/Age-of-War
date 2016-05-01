#ifndef PauseState_H_
#define PauseState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/Textbox.h>

class PauseState : public pyro::State
{
private:
	enum ButtonType { Resume, Restart, Quit_Menu, Quit_Desktop, TypeCount };
	using Buttons = std::vector<pyro::gui::ClickableGUIEntity>;
private:
	sf::Font        mFont;
	Buttons         mButtons;

	sf::VertexArray mBackgroundBlur;
	sf::VertexArray mBackground;

public:
	PauseState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	virtual bool handleEvent(const sf::Event& event);
	virtual bool update(sf::Time dt);
	virtual void draw();
};
#endif