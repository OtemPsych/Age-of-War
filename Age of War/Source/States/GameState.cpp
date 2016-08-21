#include "GameState.h"
#include "GameOverState.h"
#include "../DataTables.h"

#include <PYRO/StateStack.h>

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack* stack, sf::RenderWindow* window)
	: State(stack, window)
	, cursor_(nullptr)
	, base_player_(nullptr)
	, base_opponent_(nullptr)
	, window_bounds_(0, 0, window->getSize().x, window->getSize().y)
{
	setupResources();
	base_data_ = std::move(data::initBaseData(base_textures_));
	
	buildScene();
}

GameState::~GameState()
{
}

void GameState::unpauseMusic()
{
	music_player_.pause(false);
	cursor_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
}

bool GameState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		|| event.type == sf::Event::LostFocus) {
		requestStatePush(pyro::StateID::Pause);
		music_player_.pause(true);
	}
	else if (event.type == sf::Event::Closed) {
		requestStateClear();
	}

	scene_graph_.handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	if (base_player_->hasUnits())
		base_opponent_->attack(base_player_->getFirstUnit());
	else {
		base_opponent_->attack(base_player_);
		if (base_player_->isDestroyed()) {
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(stack_->getState(pyro::StateID::GameOver)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Defeat);
			} else {
				requestStatePush(pyro::StateID::GameOver);
				music_player_.stop();
			}
		}
	}

	if (base_opponent_->hasUnits())
		base_player_->attack(base_opponent_->getFirstUnit());
	else {
		base_player_->attack(base_opponent_);
		if (base_opponent_->isDestroyed()) {
			auto* gameOverState = const_cast<GameOverState*>(dynamic_cast<const GameOverState*>(stack_->getState(pyro::StateID::GameOver)));
			if (gameOverState) {
				gameOverState->setGameOverType(GameOverState::GameOverType::Victory);
			} else {
				requestStatePush(pyro::StateID::GameOver);
				music_player_.stop();
			}
		}
	}

	cursor_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));

	scene_graph_.update(dt);

	return true;
}

void GameState::draw()
{
	window_->draw(scene_graph_);
}

void GameState::buildScene()
{
	// Init Scene Layers
	for (unsigned i = 0; i < SceneLayers::SceneLayerCount; i++) {
		auto scene_layer(std::make_unique<pyro::SceneNode>());
		scene_layers_.push_back(scene_layer.get());
		scene_graph_.attachChild(std::move(scene_layer));
	}

	// Init Background to Background Layer
	const sf::Vector2f texture_size(background_texture_.getSize());
	const sf::FloatRect background_texture_rect(0.f, 0.f, texture_size.x * 1.5f, texture_size.y);
	auto background(std::make_unique<pyro::VertexArrayNode>(background_texture_));
	background->setTextureRect(background_texture_rect);
	background->scale(1.f, window_bounds_.height / texture_size.y);
	scene_layers_[SceneLayers::Background]->attachChild(std::move(background));

	world_bounds_ = background_texture_rect;

	// Init Bases to Bases Layer
	auto base_player(std::make_unique<BasePlayer>(world_bounds_, damage_font_, unit_textures_,
		                                          turret_textures_, Entity::Side::Ally, window_,
											      base_data_[Base::BaseType::DefaultBase].get(),
											      &sound_player_, &scene_layers_));
	base_player_ = base_player.get();
	scene_layers_[SceneLayers::Bases]->attachChild(std::move(base_player));

	auto base_opponent(std::make_unique<BaseAI>(world_bounds_, damage_font_, unit_textures_,
		                                        turret_textures_, Entity::Side::Enemy,
		                                        base_data_[Base::BaseType::DefaultBase].get(),
											    &sound_player_, &scene_layers_));
	base_opponent_ = base_opponent.get();
	scene_layers_[SceneLayers::Bases]->attachChild(std::move(base_opponent));

	// Init Cursor to Cursor Layer
	auto cursor(std::make_unique<pyro::VertexArrayNode>(cursor_texture_));
	cursor->scale(0.9f, 0.9f);
	cursor_ = cursor.get();
	scene_layers_[SceneLayers::Cursor]->attachChild(std::move(cursor));

	music_player_.setVolume(75.f);
	music_player_.play(MusicID::Soundtrack, true);
}

void GameState::setupResources()
{
	unit_textures_.load("Assets/Textures/Mage.png", Unit::UnitType::Mage);
	unit_textures_.load("Assets/Textures/Knight.png", Unit::UnitType::Knight);
	unit_textures_.load("Assets/Textures/Destroyer.png", Unit::UnitType::Destroyer);
	unit_textures_.load("Assets/Textures/Executioner.png", Unit::UnitType::Executioner);
	unit_textures_.load("Assets/Textures/Shadow.png", Unit::UnitType::Shadow);
	unit_textures_.load("Assets/Textures/Samurai.png", Unit::UnitType::Samurai);

	turret_textures_.load("Assets/Textures/LaserTurret.png", Turret::LaserTurret);

	cursor_texture_.loadFromFile("Assets/Textures/MouseCursor.png");
	cursor_texture_.setSmooth(true);
	background_texture_.loadFromFile("Assets/Textures/Background.png");
	background_texture_.setRepeated(true);
	base_textures_.load("Assets/Textures/Base.png", Base::BaseType::DefaultBase);

	damage_font_.loadFromFile("Assets/Fonts/Zombie.ttf");

	music_player_.loadTheme(MusicID::Soundtrack, "Assets/Music/Soundtrack.ogg");
	sound_player_.loadEffect(Unit::SoundID::MageAttack, "Assets/Sounds/MageAttack.ogg");
	sound_player_.loadEffect(Unit::SoundID::KnightAttack, "Assets/Sounds/KnightAttack.ogg");
}