#ifndef MultiplayerConnectState_H_
#define MultiplayerConnectState_H_

#include <PYRO/State.h>
#include <PYRO/GUI/Textbox.h>

#include <SFML/Graphics/Sprite.hpp>

class MultiplayerConnectState : public pyro::State
{
private:
	sf::Font		   mFont;

	sf::Sprite  mBackgroundSprite;
	sf::Texture mBackgroundTexture;

	pyro::gui::Textbox            mIpTextbox;
	pyro::gui::Textbox            mPortTextbox;
	pyro::gui::ClickableGUIEntity mConnectButton;

private:
	void setupGUIEntities();
	virtual void setupResources() override;
public:
	MultiplayerConnectState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	virtual bool handleEvent(const sf::Event& event) override;
	virtual void draw() override;

	std::string getHostIP();
	std::string getHostPort();
};
#endif