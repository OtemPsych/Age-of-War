#include "BasePlayer.h"
#include "Tools/TextureDataReader.h"
#include "DataTables.h"
#include "States/GameState.h"

BasePlayer::BasePlayer(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
	                   const pyro::TextureHolder<Unit::UnitType>& unit_textures,
	                   const pyro::TextureHolder<Turret::TurretType>& turret_textures,
	                   Side side, sf::RenderWindow* window, data::BaseData* base_data,
	                   pyro::SoundPlayer<Unit::SoundID>* sound_player,
					   const std::vector<pyro::SceneNode*>* scene_layers)
	: Base(world_bounds, damage_font, unit_textures, turret_textures, side, base_data, sound_player, scene_layers)
	, window_(window)
	, coin_(nullptr)
	, coin_text_(nullptr)
	, coin_rotation_animation_(nullptr)
	, unit_buttons_(nullptr)
	, turret_buttons_(nullptr)
	, unit_upgrade_buttons_(nullptr)
	, turret_upgrade_buttons_(nullptr)
	, active_turret_placement_indicators_(false)
	, turret_indicator_(nullptr)
	, turret_placement_indicators_(nullptr)
{
	init(world_bounds);
}

void BasePlayer::modifyGold(int amount)
{
	gold_ += amount;
	updateGoldGUI();
}

void BasePlayer::init(const sf::FloatRect& world_bounds)
{
	// Init Camera
	auto camera(std::make_unique<Camera>(world_bounds, window_));

	// Init Spawn Buttons
	auto unit_buttons(std::make_unique<UnitSpawnButtons>(unit_data_, unit_textures_, sf::Vector2f(65.f, 70.f), window_));
	unit_buttons->setPosition(window_->getSize().x / 2.f, 10.f);
	unit_buttons_ = unit_buttons.get();

	auto turret_buttons(std::make_unique<TurretSpawnButtons>(turret_data_, turret_textures_, sf::Vector2f(65.f, 30.f), window_));
	turret_buttons->setPosition(window_->getSize().x / 4.f, 10.f);
	turret_buttons_ = turret_buttons.get();

	// Init Upgrade Buttons
	auto unit_upgrade_buttons(std::make_unique<UnitUpgradeButtons>(unit_data_, unit_buttons_->getButtonProperties(),
		window_));
	unit_upgrade_buttons_ = unit_upgrade_buttons.get();
	camera->attachChild(std::move(unit_upgrade_buttons));
	camera->attachChild(std::move(unit_buttons));

	auto turret_upgrade_buttons(std::make_unique<TurretUpgradeButtons>(turret_data_, turret_buttons_->getButtonProperties(),
		window_));
	turret_upgrade_buttons_ = turret_upgrade_buttons.get();
	camera->attachChild(std::move(turret_upgrade_buttons));
	camera->attachChild(std::move(turret_buttons));

	// Init Coin
	coin_texture_.loadFromFile("Assets/Textures/Coin.png");
	auto coin(std::make_unique<pyro::SpriteNode>(coin_texture_));
	coin->setPosition(30.f, 30.f);

	coin_font_.loadFromFile("Assets/Fonts/Gold.ttf");
	auto coin_text(std::make_unique<pyro::Text>());
	coin_text->setFont(coin_font_);
	coin_text->setCharacterSize(45);
	coin_text->setTextColor(sf::Color::Black);
	coin_text->activateShadow(true);
	coin_text->setShadowOffset(3.f, 2.f);
	coin_text->setShadowColor(sf::Color(255, 255, 255, 150));
	coin_text->setOriginFlags(pyro::utils::OriginFlag::Left | pyro::utils::OriginFlag::CenterY);
	coin_text->setPosition(60.f, coin->getPosition().y);

	coin_rotation_animation_data_.frames = std::move(readTextureData("Coin", "Rotate"));
	coin_rotation_animation_data_.repeat = true;
	coin_rotation_animation_data_.total_duration = sf::seconds(0.8f);
	data::setupIndividualAnimation(&coin_rotation_animation_data_);
	auto coin_rotation_animation(std::make_unique<Animation>(coin_rotation_animation_data_, coin.get()));
	coin->attachChild(std::move(coin_rotation_animation));

	coin_ = coin.get();
	coin_text_ = coin_text.get();
	camera->attachChild(std::move(coin));
	camera->attachChild(std::move(coin_text));

	// Init Turret Placement Indicators
	auto turret_placement_indicators(std::make_unique<gui::TurretPlacementIndicators>(turret_window_rects_, window_));
	turret_placement_indicators->activateDrawing(ActivationTarget::Both, false);
	turret_placement_indicators->setPosition(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f + 15.f);

	turret_placement_indicators_ = turret_placement_indicators.get();
	attachChild(std::move(turret_placement_indicators));

	scene_layers_->at(GameState::Gui)->attachChild(std::move(camera));

	updateGoldGUI();
}

void BasePlayer::updateGoldGUI()
{
	unit_buttons_->updateButtonOverlay(gold_);
	turret_buttons_->updateButtonOverlay(gold_);

	coin_text_->setString(std::to_string(gold_));
}

void BasePlayer::handleEventCurrent(const sf::Event& event)
{
	int i = unit_buttons_->getTypeClicked();
	if (i >= 0)
		handleUnitSpawn(static_cast<Unit::UnitType>(i));

	int j = turret_buttons_->getTypeClicked();
	if (j >= 0 && gold_ >= turret_data_[j]->cost) {
		active_turret_placement_indicators_ = true;
		turret_type_to_spawn_ = j;
		turret_placement_indicators_->activateDrawing(ActivationTarget::Both, true);

		auto turret_indicator = std::move(turret_buttons_->getButtonBox(j));
		sf::FloatRect lbounds(turret_indicator->getLocalBounds());
		turret_indicator->setOrigin(lbounds.width / 3.f, lbounds.height / 3.f);
		turret_indicator_ = turret_indicator.get();
		scene_layers_->at(GameState::Gui)->attachChild(std::move(turret_indicator));
	}

	if (active_turret_placement_indicators_ && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
		if (turret_placement_indicators_->getTurretIndicator() != -1) {
			handleTurretSpawn(static_cast<Turret::TurretType>(turret_type_to_spawn_), turret_placement_indicators_->getTurretIndicator());
			active_turret_placement_indicators_ = false;
			turret_type_to_spawn_ = -1;
			scene_layers_->at(GameState::Gui)->detachChild(*turret_indicator_);
			turret_indicator_ = nullptr;
			turret_placement_indicators_->activateDrawing(ActivationTarget::Both, false);
		}
		else {
			active_turret_placement_indicators_ = false;
			scene_layers_->at(GameState::Gui)->detachChild(*turret_indicator_);
			turret_indicator_ = nullptr;
			turret_placement_indicators_->activateDrawing(ActivationTarget::Both, false);
		}
	}
}

void BasePlayer::updateCurrent(sf::Time dt)
{
	Base::updateCurrent(dt);
	if (turret_indicator_) {
		turret_indicator_->setPosition(window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)));
	}
}