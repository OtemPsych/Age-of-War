#ifndef PauseState_H_
#define PauseState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/Button.h>
#include <PYRO/VertexArrayNode.h>

class PauseState : public pyro::State
{
private:
	enum SceneLayers { BackgroundLayer, ButtonLayer, CursorLayer, SceneLayerCount };
	enum ButtonType { Resume, Restart, Quit_Menu, Quit_Desktop, TypeCount };

public:
	PauseState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;
private:
	void init();
	virtual void setupResources() override;

private:
	pyro::SceneNode                 scene_graph_;
	std::vector<pyro::SceneNode*>   scene_layers_;
					  
	std::vector<pyro::gui::Button*> buttons_;
	pyro::VertexArrayNode*          cursor_;		              
					                
	sf::Font                        font_;
	sf::Texture                     cursor_texture_;
};
#endif