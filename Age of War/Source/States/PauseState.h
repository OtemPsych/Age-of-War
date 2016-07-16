#ifndef PauseState_H_
#define PauseState_H_

#include <SFML/Graphics/Sprite.hpp>

#include <PYRO/State.h>
#include <PYRO/GUI/Textbox.h>

class PauseState : public pyro::State
{
private:
	enum ButtonType { Resume, Restart, Quit_Menu, Quit_Desktop, TypeCount };
	using Buttons = std::vector<pyro::gui::ClickableGUIEntity*>;

public:
	PauseState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
private:
	void initBackgroundBlur(const sf::FloatRect& view);
	void initBackground(const sf::FloatRect& view, const sf::Vector2f& button_size,
		                float margin);

private:
	pyro::SceneNode scene_graph_;

	sf::Font        font_;
	Buttons         buttons_;

	sf::VertexArray background_blur_;
	sf::VertexArray background_;

	sf::Sprite      cursor_;
	sf::Texture     cursor_texture_;
};
#endif