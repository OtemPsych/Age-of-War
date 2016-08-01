#include <PYRO/StateStack.h>
#include <PYRO/Utils.h>

#include "MenuState.h"

MenuState::MenuState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, cursor_(nullptr)
{
	setupResources();
	init();
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		cursor_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	}
	else if (buttons_[Play]->clicked(event)) {
		requestStatePop();
		requestStatePush(pyro::StateID::Game);
	} 
	else if (buttons_[Multiplayer]->clicked(event)) {
		requestStatePop();
		requestStatePush(pyro::StateID::MultiplayerConnect);
	} 
	else if (event.type == sf::Event::Closed || buttons_[Quit]->clicked(event)) {
		requestStateClear();
	}

	scene_graph_.handleEvent(event);

	return true;
}

bool MenuState::update(sf::Time dt)
{
	for (auto& button : buttons_) {
		if (button->hover()) {
			button->setFillColor(sf::Color(255, 255, 255, 120));
			break;
		}
		else {
			button->setFillColor(sf::Color(0, 0, 0, 150));
		}
	}

	scene_graph_.update(dt);

	return true;
}

void MenuState::draw()
{
	window_->draw(scene_graph_);
}

void MenuState::init()
{
	window_->setMouseCursorVisible(false);

	// Init Scene Layers
	for (unsigned i = 0; i < SceneLayerCount; i++) {
		auto scene_layer(std::make_unique<pyro::SceneNode>());
		scene_layers_.push_back(scene_layer.get());
		scene_graph_.attachChild(std::move(scene_layer));
	}

	// Init Cursor
	auto cursor(std::make_unique<pyro::VertexArrayNode>(cursor_texture_));
	cursor->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	cursor->scale(0.9f, 0.9f);
	cursor_ = cursor.get();
	scene_layers_[CursorLayer]->attachChild(std::move(cursor));

	// Init Background
	const sf::Vector2f win_size(window_->getSize());
	const sf::Vector2f texture_size(background_texture_.getSize());

	auto background(std::make_unique<pyro::VertexArrayNode>(background_texture_));
	background->scale(win_size.x / texture_size.x, win_size.y / texture_size.y);
	scene_layers_[BackgroundLayer]->attachChild(std::move(background));

	// Init Buttons
	const sf::Vector2f button_size(185.f, 50.f);
	const float margin = 15.f;
	const sf::Vector2f half_win_size(win_size / 2.f);
	const sf::Vector2f half_button_size(button_size / 2.f);

	for (unsigned i = 0; i < ButtonTypeCount; i++) {
		auto button(std::make_unique<pyro::gui::Button>(button_size, window_));
		button->setTexture(&button_texture_);
		button->setPosition(half_win_size.x - half_button_size.x,
			                half_win_size.y + (button_size.y + margin) * i - half_button_size.y);
		button->activateText(true);

		pyro::Text* text = button->getText();
		text->setFont(font_);
		text->setTextColor(sf::Color::White);
		text->activateShadow(true);
		text->setShadowOffset(2.f, 2.f);
		text->setShadowColor(sf::Color(255, 255, 255, 50));
		text->setOriginFlags(pyro::utils::OriginFlag::Center);
		pyro::utils::setRelativeAlignment(*button, pyro::utils::OriginFlag::Center, 0.f, text);

		buttons_.push_back(button.get());
		scene_layers_[ButtonLayer]->attachChild(std::move(button));
	}

	buttons_[Play]->getText()->setString("Play");
	buttons_[Multiplayer]->getText()->setString("Multiplayer");
	buttons_[Quit]->getText()->setString("Quit");
}

void MenuState::setupResources()
{
	// Fonts
	font_.loadFromFile("Assets/Fonts/Menu.ttf");
	// Textures
	cursor_texture_.loadFromFile("Assets/Textures/MouseCursor.png");
	button_texture_.loadFromFile("Assets/Textures/MenuButton.png");
	background_texture_.loadFromFile("Assets/Textures/MenuBackground.jpg");
}