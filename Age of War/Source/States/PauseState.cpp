#include <PYRO/StateStack.h>
#include <PYRO/Utils.h>

#include "PauseState.h"
#include "GameState.h"

PauseState::PauseState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, cursor_(nullptr)
{
	setupResources();
	init();
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		cursor_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	}
	else if (buttons_[Resume]->clicked(event) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
		requestStatePop();
		const_cast<GameState*>(dynamic_cast<const GameState*>(stack_->getState(pyro::StateID::Game)))->unpauseMusic();
	} 
	else if (buttons_[Restart]->clicked(event)) {
		window_->setView(window_->getDefaultView());

		requestStateClear();
		requestStatePush(pyro::StateID::Game);
	} 
	else if (buttons_[Quit_Menu]->clicked(event)) {
		window_->setView(window_->getDefaultView());

		requestStateClear();
		requestStatePush(pyro::StateID::Menu);
	} 
	else if (event.type == sf::Event::Closed || buttons_[Quit_Desktop]->clicked(event)) {
		requestStateClear();
	}

	return false;
}

bool PauseState::update(sf::Time dt)
{
	for (auto& button : buttons_)
		if (button->hover()) {
			button->getText()->setTextColor(sf::Color(255, 255, 255, 255));
			break;
		} 
		else {
			button->getText()->setTextColor(sf::Color(255, 255, 255, 150));
		}

	return false;
}

void PauseState::draw()
{
	window_->draw(scene_graph_);
}

void PauseState::init()
{
	// Init Scene Layers
	for (int i = 0; i < SceneLayers::SceneLayerCount; i++) {
		auto scene_layer(std::make_unique<pyro::SceneNode>());
		scene_layers_.push_back(scene_layer.get());
		scene_graph_.attachChild(std::move(scene_layer));
	}

	// Init Cursor
	auto cursor(std::make_unique<pyro::VertexArrayNode>(cursor_texture_));
	cursor->scale(0.9f, 0.9f);
	cursor->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	cursor_ = cursor.get();
	scene_layers_[CursorLayer]->attachChild(std::move(cursor));

	// Init Buttons
	const sf::Vector2f view_size(window_->getSize());
	const sf::Vector2f view_center(window_->getView().getCenter());
	const sf::FloatRect view(view_center - view_size / 2.f, view_size);
	const sf::Vector2f button_size(350.f, 50.f);
	const float margin = 15.f;
	for (unsigned i = 0; i < TypeCount; i++) {
		auto button(std::make_unique<pyro::gui::Button>(button_size, window_));
		button->setFillColor(sf::Color::Transparent);
		button->setPosition(view_center.x - button_size.x / 2.f,
			                view_center.y + (button_size.y + margin) * i - button_size.y / 2.f);
		button->activateText(true);

		pyro::Text* text = button->getText();
		text->setFont(font_);
		text->setShadowOffset(2.f, 2.f);
		text->setShadowColor(sf::Color(255, 255, 255, 45));
		text->setOriginFlags(pyro::utils::OriginFlag::Center);
		text->setTextColor(sf::Color(255, 255, 255, 150));
		text->setPosition(button->getSize() / 2.f);

		buttons_.push_back(button.get());
		scene_layers_[ButtonLayer]->attachChild(std::move(button));
	}

	buttons_[Resume]->getText()->setString("Resume");
	buttons_[Restart]->getText()->setString("Restart");
	buttons_[Quit_Menu]->getText()->setString("Quit to Main Menu");
	buttons_[Quit_Desktop]->getText()->setString("Quit to Desktop");

	// Init Background
	auto background_blur(std::make_unique<pyro::VertexArrayNode>(sf::Vector2f(view.width, view.height)));
	background_blur->setFillColor(sf::Color(0, 0, 0, 150));
	background_blur->setPosition(view.left, 0.f);
	scene_layers_[BackgroundLayer]->attachChild(std::move(background_blur));

	const float top_pos_y = buttons_.front()->getPosition().y - margin;
	const float bot_pos_y = buttons_.back()->getPosition().y + button_size.y + margin;
	auto background(std::make_unique<pyro::VertexArrayNode>(sf::Vector2f(view.width - 800.f, bot_pos_y - top_pos_y)));
	background->setFillColor(sf::Color(0, 0, 0, 200));
	background->setOriginFlags(pyro::utils::OriginFlag::CenterX | pyro::utils::OriginFlag::Top);
	background->setPosition(view.left + view.width / 2.f, top_pos_y);

	background->vertices_.append(sf::Vertex(sf::Vector2f(-400.f, 0.f),                   sf::Color(0, 0, 0, 0)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(0.f,    0.f),                   sf::Color(0, 0, 0, 200)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(0.f,    bot_pos_y - top_pos_y), sf::Color(0, 0, 0, 200)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(-400.f, bot_pos_y - top_pos_y), sf::Color(0, 0, 0, 0)));

	background->vertices_.append(sf::Vertex(sf::Vector2f(background->vertices_[1].position.x, 0.f),                   sf::Color(0, 0, 0, 200)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(view.width - 400.f,                  0.f),                   sf::Color(0, 0, 0, 0)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(view.width - 400.f,                  bot_pos_y - top_pos_y), sf::Color(0, 0, 0, 0)));
	background->vertices_.append(sf::Vertex(sf::Vector2f(background->vertices_[1].position.x, bot_pos_y - top_pos_y), sf::Color(0, 0, 0, 200)));
	scene_layers_[BackgroundLayer]->attachChild(std::move(background));
}

void PauseState::setupResources()
{
	// Textures
	cursor_texture_.loadFromFile("Assets/Textures/MouseCursor.png");

	// Fonts
	font_.loadFromFile("Assets/Fonts/Menu.ttf");
}