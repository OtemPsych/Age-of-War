#include <PYRO/StateStack.h>
#include <PYRO/Utils.h>

#include "PauseState.h"
#include "GameState.h"

PauseState::PauseState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, background_blur_(sf::Quads, 4)
	, background_(sf::Quads, 4)
{
	cursor_texture_.loadFromFile("Assets/Textures/MouseCursor.png");
	cursor_.setTexture(cursor_texture_);
	cursor_.scale(0.9f, 0.9f);
	cursor_.setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));

	font_.loadFromFile("Assets/Fonts/Menu.ttf");

	const sf::Vector2f view_size(window_->getSize());
	const sf::Vector2f view_center(window_->getView().getCenter());
	const sf::FloatRect view(view_center - view_size / 2.f, view_size);
	const sf::Vector2f button_size(350.f, 50.f);
	const float margin = 15.f;
	for (unsigned i = 0; i < TypeCount; i++)
	{
		auto button(std::make_unique<pyro::gui::ClickableGUIEntity>(button_size, window_));

		button->activateText(true);
		pyro::Text* text = button->getText();
		text->setFont(font_);
		text->setShadowOffset(2.f, 2.f);
		text->setShadowColor(sf::Color(255, 255, 255, 45));
		text->setOriginFlags(pyro::utils::OriginFlag::Center);
		text->setTextColor(sf::Color(255, 255, 255, 150));
		text->setPosition(button->getSize() / 2.f);
		button->setFillColor(sf::Color::Transparent);
		button->setPosition(view_center.x - button_size.x / 2.f,
			                view_center.y + (button_size.y + margin) * i - button_size.y / 2.f);

		buttons_.push_back(button.get());
		scene_graph_.attachChild(std::move(button));
	}

	buttons_[Resume]->getText()->setString("Resume");
	buttons_[Restart]->getText()->setString("Restart");
	buttons_[Quit_Menu]->getText()->setString("Quit to Main Menu");
	buttons_[Quit_Desktop]->getText()->setString("Quit to Desktop");

	initBackgroundBlur(view);
	initBackground(view, button_size, margin);
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		cursor_.setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
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

	scene_graph_.handleEvent(event);

	return false;
}

bool PauseState::update(sf::Time dt)
{
	for (auto& button : buttons_) {
		if (button->hover()) {
			button->getText()->setTextColor(sf::Color(255, 255, 255, 255));
			break;
		} 
		else {
			button->getText()->setTextColor(sf::Color(255, 255, 255, 150));
		}
	}

		scene_graph_.update(dt);

	return false;
}

void PauseState::draw()
{
	window_->draw(background_blur_);
	window_->draw(background_);

	window_->draw(scene_graph_);

	window_->draw(cursor_);
}

void PauseState::initBackgroundBlur(const sf::FloatRect& view)
{
	background_blur_[0].position = sf::Vector2f(view.left, view.top);
	background_blur_[1].position = sf::Vector2f(view.left + view.width, view.top);
	background_blur_[2].position = sf::Vector2f(view.left + view.width, view.top + view.height);
	background_blur_[3].position = sf::Vector2f(view.left, view.top + view.height);

	for (unsigned i = 0; i < 4; i++)
		background_blur_[i].color = sf::Color(0, 0, 0, 150);
}

void PauseState::initBackground(const sf::FloatRect& view, const sf::Vector2f& button_size,
	                            float margin)
{
	const float topPosY = buttons_.front()->getPosition().y - margin;
	const float botPosY = buttons_.back()->getPosition().y + button_size.y + margin;

	background_[0].position = sf::Vector2f(view.left, topPosY);
	background_[1].position = sf::Vector2f(view.left + view.width, topPosY);
	background_[2].position = sf::Vector2f(view.left + view.width, botPosY);
	background_[3].position = sf::Vector2f(view.left, botPosY);

	for (unsigned i = 0; i < 4; i++)
		background_[i].color = sf::Color(0, 0, 0, 200);
}