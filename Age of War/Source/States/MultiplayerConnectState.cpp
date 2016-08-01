#include <PYRO/Utils.h>

#include "MultiplayerConnectState.h"

MultiplayerConnectState::MultiplayerConnectState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, cursor_(nullptr)
{
	setupResources();
	init();
}

bool MultiplayerConnectState::handleEvent(const sf::Event& event)
{
	scene_graph_.handleEvent(event);

	if (event.type == sf::Event::MouseMoved) {
		cursor_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	}
	else if (event.type == sf::Event::Closed) {
		requestStateClear();
		return false;
	}

	if (buttons_[ButtonType::Back]->clicked(event)) {
		requestStateClear();
		requestStatePush(pyro::StateID::Menu);
		return false;
	}

	if (buttons_[ButtonType::Connect]->clicked(event)) {
		requestStatePush(pyro::StateID::Multiplayer);
		return false;
	}

	return true;
}

bool MultiplayerConnectState::update(sf::Time dt)
{
	scene_graph_.update(dt);

	if (buttons_[ButtonType::Connect]->hover()) {
		buttons_[ButtonType::Connect]->setFillColor(sf::Color(155, 255, 155, 255));
	}
	else if (buttons_[ButtonType::Back]->hover()) {
		sf::VertexArray& back_vertices = buttons_[ButtonType::Back]->getVertices();
		back_vertices[0].color =
		back_vertices[1].color =
		back_vertices[2].color =
		back_vertices[3].color = sf::Color(255, 255, 255, 255);
	}
	else {
		buttons_[ButtonType::Connect]->setFillColor(sf::Color(0, 0, 0, 150));

		sf::VertexArray& back_vertices = buttons_[ButtonType::Back]->getVertices();
		back_vertices[0].color =
		back_vertices[1].color =
		back_vertices[2].color =
		back_vertices[3].color = sf::Color(255, 255, 255, 80);
	}

	return true;
}

void MultiplayerConnectState::draw()
{
	window_->draw(scene_graph_);
}

const std::string MultiplayerConnectState::getHostIP()
{
	return textboxes_[TextboxType::Ip]->getText()->getString();
}

const std::string MultiplayerConnectState::getHostPort()
{
	return textboxes_[TextboxType::Port]->getText()->getString();
}

void MultiplayerConnectState::init()
{
	// Init Scene Layers
	for (unsigned i = 0; i < SceneLayerCount; i++) {
		auto scene_layer(std::make_unique<pyro::SceneNode>());
		scene_layers_.push_back(scene_layer.get());
		scene_graph_.attachChild(std::move(scene_layer));
	}

	// Init Background
	const sf::Vector2f win_size(window_->getSize());
	const sf::Vector2f texture_size(background_texture_.getSize());
	auto background(std::make_unique<pyro::VertexArrayNode>(background_texture_));
	background->scale(win_size.x / texture_size.x, win_size.y / texture_size.y);
	scene_layers_[BackgroundLayer]->attachChild(std::move(background));

	// Init Cursor
	auto cursor(std::make_unique<pyro::VertexArrayNode>(cursor_texture_));
	cursor->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	cursor->scale(0.9f, 0.9f);
	cursor_ = cursor.get();
	scene_layers_[CursorLayer]->attachChild(std::move(cursor));

	// Init IP Textbox
	auto ip_textbox(std::make_unique<pyro::gui::Textbox>(window_));
	ip_textbox->activateText(true);
	pyro::Text* ip_text = ip_textbox->getText();
	ip_text->setFont(font_);
	ip_text->activateShadow(true);
	ip_text->setShadowOffset(2.f, 2.f);
	ip_text->setShadowColor(sf::Color(0, 0, 0, 80));
	ip_text->setOriginFlags(pyro::utils::OriginFlag::Center);

	sf::VertexArray& ip_vertices = ip_textbox->getVertices();
	ip_vertices.setPrimitiveType(sf::LinesStrip);
	ip_vertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(245.f, 0.f),  sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(250.f, 5.f),  sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(250.f, 30.f), sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(245.f, 35.f), sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(5.f,   35.f), sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(0.f,   30.f), sf::Color(158, 187, 236)));
	ip_vertices.append(sf::Vertex(sf::Vector2f(0.f,   0.f),  sf::Color(158, 187, 236)));
	ip_textbox->alignText(pyro::utils::OriginFlag::Center, 2.f);
	ip_textbox->setCaretColor(sf::Color(255, 255, 204));
	ip_textbox->setPosition(win_size / 2.f - ip_textbox->getSize() / 2.f);

	textboxes_.push_back(ip_textbox.get());
	scene_layers_[GuiLayer]->attachChild(std::move(ip_textbox));

	// Init Port Textbox
	auto port_textbox(std::make_unique<pyro::gui::Textbox>(window_));
	port_textbox->activateText(true);
	pyro::Text* port_text = port_textbox->getText();
	port_text->setFont(font_);
	port_text->activateShadow(true);
	port_text->setShadowOffset(*ip_text->getShadowOffset());
	port_text->setShadowColor(*ip_text->getShadowColor());
	port_text->setOriginFlags(pyro::utils::OriginFlag::Center);

	sf::VertexArray& port_vertices = port_textbox->getVertices();
	port_vertices.setPrimitiveType(sf::LinesStrip);
	port_vertices.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(135.f, 0.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(140.f, 5.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(140.f, 30.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(135.f, 35.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(5.f, 35.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(0.f, 30.f), sf::Color(158, 187, 236)));
	port_vertices.append(sf::Vertex(sf::Vector2f(0.f, 0.f), sf::Color(158, 187, 236)));
	port_textbox->alignText(pyro::utils::OriginFlag::Center, 2.f);
	port_textbox->setCaretColor(sf::Color(125, 166, 181));
	port_textbox->setPosition(win_size.x / 2.f - port_textbox->getSize().x / 2.f,
		                      textboxes_[TextboxType::Ip]->getPosition().y + port_textbox->getGlobalBounds().height * 1.5f);

	textboxes_.push_back(port_textbox.get());
	scene_layers_[GuiLayer]->attachChild(std::move(port_textbox));

	// Connect Button
	auto connect_button(std::make_unique<pyro::gui::Button>(sf::Vector2f(150.f, 45.f), window_));
	connect_button->activateText(true);
	pyro::Text* connect_text = connect_button->getText();
	connect_text->setFont(font_);
	connect_text->setString("Connect");
	connect_text->activateShadow(true);
	connect_text->setShadowOffset(*ip_text->getShadowOffset());
	connect_text->setShadowColor(*ip_text->getShadowColor());
	connect_text->setOriginFlags(pyro::utils::OriginFlag::Center);

	connect_button->setPosition(win_size.x / 2.f - connect_button->getSize().x / 2.f,
		                        textboxes_[TextboxType::Port]->getPosition().y 
								+ textboxes_[TextboxType::Port]->getGlobalBounds().height * 2.5f);
	connect_button->setTexture(&connect_button_texture_);
	connect_button->alignText(pyro::utils::OriginFlag::Center, 2.f);

	buttons_.push_back(connect_button.get());
	scene_layers_[GuiLayer]->attachChild(std::move(connect_button));

	// Back Button
	auto back_button(std::make_unique<pyro::gui::Button>(window_));
	back_button->activateText(true);
	pyro::Text* back_text = back_button->getText();
	back_text->setFont(font_);
	back_text->setString("Back");
	back_text->activateShadow(true);
	back_text->setShadowOffset(*ip_text->getShadowOffset());
	back_text->setShadowColor(*ip_text->getShadowColor());
	back_text->setOriginFlags(pyro::utils::OriginFlag::Left | pyro::utils::OriginFlag::CenterY);
	back_text->setPosition(sf::Vector2f(25.f, 15.f));

	sf::VertexArray& back_vertices = back_button->getVertices();
	back_vertices.setPrimitiveType(sf::Quads);
	back_vertices.append(sf::Vertex(sf::Vector2f(0.f, 15.f), sf::Color::White));
	back_vertices.append(sf::Vertex(sf::Vector2f(15.f, 0.f), sf::Color::White));
	back_vertices.append(sf::Vertex(sf::Vector2f(15.f, 15.f), sf::Color::White));
	back_vertices.append(sf::Vertex(sf::Vector2f(15.f, 30.f), sf::Color::White));
	back_vertices.append(sf::Vertex(sf::Vector2f(15.f, 0.f), sf::Color::Transparent));
	back_vertices.append(sf::Vertex(sf::Vector2f(95.f, 0.f), sf::Color::Transparent));
	back_vertices.append(sf::Vertex(sf::Vector2f(95.f, 30.f), sf::Color::Transparent));
	back_vertices.append(sf::Vertex(sf::Vector2f(15.f, 30.f), sf::Color::Transparent));
	back_button->setPosition(15.f + 5.f, win_size.y - back_button->getGlobalBounds().height - 15.f);

	buttons_.push_back(back_button.get());
	scene_layers_[GuiLayer]->attachChild(std::move(back_button));
}

void MultiplayerConnectState::setupResources()
{
	// Fonts
	font_.loadFromFile("Assets/Fonts/Menu.ttf");
	// Textures
	connect_button_texture_.loadFromFile("Assets/Textures/MenuButton.png");
	background_texture_.loadFromFile("Assets/Textures/MenuBackground.jpg");
	cursor_texture_.loadFromFile("Assets/Textures/MouseCursor.png");
}