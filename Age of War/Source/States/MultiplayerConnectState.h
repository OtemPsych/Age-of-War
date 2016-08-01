#ifndef MultiplayerConnectState_H_
#define MultiplayerConnectState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/Textbox.h>
#include <PYRO/VertexArrayNode.h>

class MultiplayerConnectState : public pyro::State
{
private:
	enum SceneLayers { BackgroundLayer, GuiLayer, CursorLayer, SceneLayerCount };
	enum TextboxType { Ip, Port, TextboxCount };
	enum ButtonType { Connect, Back, ButtonCount };

public:
	MultiplayerConnectState(pyro::StateStack* stack, sf::RenderWindow* window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual bool update(sf::Time dt) override;
	virtual void draw() override;

	const std::string getHostIP();
	const std::string getHostPort();
private:
	void init();
	virtual void setupResources() override;

private:
	pyro::SceneNode                  scene_graph_;
	std::vector<pyro::SceneNode*>    scene_layers_;

	std::vector<pyro::gui::Textbox*> textboxes_;
	std::vector<pyro::gui::Button*>  buttons_;
	pyro::VertexArrayNode*           cursor_;

	sf::Font                         font_;
	sf::Texture                      connect_button_texture_;
	sf::Texture                      background_texture_;
	sf::Texture                      cursor_texture_;
};
#endif