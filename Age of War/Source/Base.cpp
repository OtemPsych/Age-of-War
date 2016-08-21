#include "Base.h"
#include "RangedUnit.h"
#include "DataTables.h"
#include "States/GameState.h"

#include <PYRO/Math.h>

Base::Base(const sf::FloatRect& world_bounds, const sf::Font& damage_font,
	       const pyro::TextureHolder<Unit::UnitType>& unit_textures,
	       const pyro::TextureHolder<Turret::TurretType>& turret_textures,
	       Side side, data::BaseData* base_data,
	       pyro::SoundPlayer<Unit::SoundID>* sound_player,
		   const std::vector<pyro::SceneNode*>* scene_layers)
	: HealthEntity(side, base_data)
	, scene_layers_(scene_layers)
	, unit_textures_(unit_textures)
	, unit_data_(std::move(data::initUnitData(unit_textures)))
	, turret_textures_(turret_textures)
	, turret_data_(std::move(data::initTurretData(turret_textures)))
	, turret_type_to_spawn_(-1)
	, value_display_data_(std::move(data::initValueDisplayData(damage_font)))
	, gold_(base_data->gold)
	, base_data_(base_data)
	, sound_player_(sound_player)
	, damage_font_(damage_font)
	, spawn_bar_(nullptr)
	, mp_unit_type_(-1)
	, mp_turret_info_(std::make_pair(-1, -1))
	, current_population_(0)
	, smoke_system_(nullptr)
{
	auto spawn_bar(std::make_unique<gui::SpawnBar>(getGlobalBounds(), true));
	if (side_ == Side::Ally) {
		setPosition(getGlobalBounds().width / 2.f + 20.f, 0.7f * world_bounds.height);
	}
	else {
		setPosition(world_bounds.width - getGlobalBounds().width / 2.f - 20.f, 0.7f * world_bounds.height);
		spawn_bar->scale(-1.f, 1.f);
	}
	spawn_bar_ = spawn_bar.get();
	attachChild(std::move(spawn_bar));

	const sf::FloatRect lbounds(getLocalBounds());
	turret_window_rects_[0] = sf::FloatRect(18.f - lbounds.width / 2.f, 103.f - lbounds.height / 2.f, 43.f, 44.f);
	turret_window_rects_[1] = sf::FloatRect(67.f - lbounds.width / 2.f, 103.f - lbounds.height / 2.f, 43.f, 44.f);
	turret_window_rects_[2] = sf::FloatRect(117.f - lbounds.width / 2.f, 103.f - lbounds.height / 2.f, 43.f, 44.f);

	auto smoke_system(std::make_unique<SmokeSystem>());
	smoke_system_ = smoke_system.get();
	attachChild(std::move(smoke_system));
}

Base::~Base()
{
}

void Base::attack(HealthEntity* enemy)
{
	for (auto& itr = units_.begin(); itr != units_.end(); ++itr) {
		if ((*itr)->getGeneralUnitType() == Unit::GeneralUnitType::Ranged || itr == units_.begin()) {
			(*itr)->attack(enemy);
			if (enemy->isDestroyed()) {
				for (auto& unit : units_)
					unit->nullifyEnemyTargeted();
				break;
			}
		}
	}

	for (auto& turret : turrets_) {
		turret.first->attack(enemy);
		if (enemy->isDestroyed()) {
			for (auto& turret : turrets_)
				turret.first->nullifyEnemyTargeted();
			break;
		}
	}

	if (enemy->isDestroyed() && enemy->getEntityType() == Entity::EntityType::Unit) {
		modifyGold(125u * unit_data_[dynamic_cast<Unit*>(enemy)->getUnitType()]->cost / 100u);
	}
}

void Base::modifyGold(int amount)
{
	gold_ += amount;
}

sf::Packet& operator<<(sf::Packet& packet, Base& base)
{
	// New Unit
	if (base.mp_unit_type_ > -1) {
		assert(packet << static_cast<sf::Uint8>(Base::PacketTypeID::NewUnit));
		assert(packet << base.mp_unit_type_);
		base.mp_unit_type_ = -1;
	}
	// New Turret
	if (base.mp_turret_info_.first > -1) {
		assert(packet << static_cast<sf::Uint8>(Base::PacketTypeID::NewTurret));
		assert(packet << base.mp_turret_info_.first);

		switch (base.mp_turret_info_.second) {
		case sf::Int8(0):
			assert(packet << sf::Int8(2));
			break;
		case sf::Int8(2):
			assert(packet << sf::Int8(0));
			break;
		default:
			assert(packet << sf::Int8(1));
		}

		base.mp_turret_info_.first = -1;
		base.mp_turret_info_.second = -1;
	}

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Base& base)
{
	if (!packet.endOfPacket()) {
		sf::Uint8 packet_type_id;
		assert(packet >> packet_type_id);

		Base::PacketTypeID packet_type_id_cast = static_cast<Base::PacketTypeID>(packet_type_id);
		if (packet_type_id_cast == Base::PacketTypeID::NewUnit) {
			assert(packet >> base.mp_unit_type_);
			base.handleUnitSpawn(static_cast<Unit::UnitType>(base.mp_unit_type_));
		}

		if (packet_type_id_cast == Base::PacketTypeID::NewTurret) {
			assert(packet >> base.mp_turret_info_.first);
			assert(packet >> base.mp_turret_info_.second);
			base.handleTurretSpawn(static_cast<Turret::TurretType>(base.mp_turret_info_.first), base.mp_turret_info_.second);
		}
	}

	return packet;
}

void Base::handleTurretSpawn(Turret::TurretType type, int turret_indicator)
{
	if (turret_indicator == -1)
		return;

	if (gold_ >= turret_data_[type]->cost) {
		for (const auto& turret : turrets_)
			if (turret.second == turret_indicator)
				return;
		
		modifyGold(-static_cast<int>(turret_data_[type]->cost));

		auto turret(std::make_unique<Turret>(damage_font_, sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height),
			                                 side_, turret_data_[type].get(), value_display_data_[0].get(),
											 scene_layers_->at(GameState::Gui), scene_layers_->at(GameState::Projectiles)));
		turret->setPosition(getPosition().x + turret_window_rects_[turret_indicator].left + turret_window_rects_[turret_indicator].width / 2.f,
			                getPosition().y + turret_window_rects_[turret_indicator].top + turret_window_rects_[turret_indicator].height / 2.f);
		turrets_.push_back(std::make_pair(turret.get(), turret_indicator));
		scene_layers_->at(GameState::SceneLayers::Units)->attachChild(std::move(turret));

		turret_type_to_spawn_ = static_cast<sf::Int16>(type);
		mp_turret_info_.first = turret_type_to_spawn_;
		mp_turret_info_.second = turret_indicator;
	}
}

void Base::handleUnitSpawn(Unit::UnitType type)
{
	if (current_population_ < base_data_->max_population && gold_ >= unit_data_[type]->cost 
		&& spawn_bar_->spawnNewUnit(gui::UnitQueue::UnitData(type, unit_data_[type]->spawn_time))) 
	{
		modifyGold(-static_cast<int>(unit_data_[type]->cost));
		mp_unit_type_ = static_cast<sf::Int16>(type);
		current_population_++;
	}
}

void Base::spawnUnit()
{
	Unit::UnitType type = spawn_bar_->getUnitTypeSpawning();

	if (unit_data_[type]->general_unit_type == Unit::GeneralUnitType::Melee) {
		auto melee_unit(std::make_unique<Unit>(side_, units_.empty() ? nullptr : units_.back(), unit_data_[type].get(),
			                                   value_display_data_[0].get(), sound_player_,
											   scene_layers_->at(GameState::SceneLayers::Gui)));

		units_.push_back(melee_unit.get());
		scene_layers_->at(GameState::SceneLayers::Units)->attachChild(std::move(melee_unit));
	}
	else {
		auto ranged_unit(std::make_unique<RangedUnit>(side_, units_.empty() ? nullptr : units_.back(),
			                                          dynamic_cast<data::RangedUnitData*>(unit_data_[type].get()),
													  value_display_data_[0].get(), sound_player_,
													  scene_layers_->at(GameState::SceneLayers::Gui),
													  scene_layers_->at(GameState::SceneLayers::Projectiles)));
		units_.push_back(ranged_unit.get());
		scene_layers_->at(GameState::SceneLayers::Units)->attachChild(std::move(ranged_unit));
	}

	const sf::Vector2f& pos = getPosition();
	const sf::FloatRect lbounds(getLocalBounds());
	units_.back()->setPosition(side_ == Side::Ally ? pos.x + lbounds.width / 2.f : pos.x - lbounds.width / 2.f,
		                       pos.y + lbounds.height / 2.f - units_.back()->getGlobalBounds().height / 2.f);
}

void Base::updateCurrent(sf::Time dt)
{
	if (spawn_bar_->isNextSpawnReady())
		spawnUnit();

	if (!units_.empty() && units_.front()->isDestroyed()) {
		if (units_.size() >= 2) {
			units_[1]->nullifyFrontUnit();
		}

		scene_layers_->at(GameState::SceneLayers::Units)->detachChild(*units_.front());
		units_.erase(units_.begin());

		current_population_--;
	}

	const sf::FloatRect lbounds(getLocalBounds());
	if (health_ <= 15 * base_data_->health.value.current / 100) {
		smoke_system_->addEmitterPosition(sf::Vector2f(-36.f + lbounds.width / 2.f, -33.f + lbounds.height / 2.f));
	}
	else if (health_ <= 32 * base_data_->health.value.current / 100) {
		smoke_system_->addEmitterPosition(sf::Vector2f(64.f + lbounds.width / 2.f, -69.f + lbounds.height / 2.f));
	}
	else if (health_ <= 55 * base_data_->health.value.current / 100) {
		smoke_system_->addEmitterPosition(sf::Vector2f(55.f + lbounds.width / 2.f, 87.f + lbounds.height / 2.f));
	}
	else if (health_ <= 80 * base_data_->health.value.current / 100) {
		smoke_system_->addEmitterPosition(sf::Vector2f(-57.f + lbounds.width / 2.f, 57.f + lbounds.height / 2.f));
	}
}

//void Base::update(sf::Time dt)
//{
//
//	for (auto& unit : mUnits)
//		unit->update(dt);
//
//	for (auto& turret : mTurrets)
//		turret.first.update(dt);
//
//	if (mHealth.current <= 15 * mHealth.original / 100) {
//		mSmokeSystem.addEmitterPosition(sf::Vector2f(-36.f, -33.f));
//	}
//	else if (mHealth.current <= 32 * mHealth.original / 100) {
//		mSmokeSystem.addEmitterPosition(sf::Vector2f(64.f, -69.f));
//	}
//	else if (mHealth.current <= 55 * mHealth.original / 100) {
//		mSmokeSystem.addEmitterPosition(sf::Vector2f(55.f, 87.f));
//	}
//	else if (mHealth.current <= 80 * mHealth.original / 100) {
//		mSmokeSystem.addEmitterPosition(sf::Vector2f(-57.f, 57.f));
//	}
//
//	mSmokeSystem.update(dt);
//}


//void Base::handleUnitUpgrade(Unit::UnitType unitType, Unit::UnitUpgradeType upgradeType)
//{
//	switch (upgradeType)
//	{
//	case Unit::UnitUpgradeType::Health:
//		if (mGold >= mUnitData[unitType].health.upgradeCost) {
//			modifyGold(-mUnitData[unitType].health.upgradeCost);
//			float currentPercentage = mUnitData[unitType].health.value.current / mUnitData[unitType].health.value.original * 100.f;
//			mUnitData[unitType].health.value.current = (currentPercentage + mUnitData[unitType].health.upgradePercentage) * mUnitData[unitType].health.value.original / 100.f;
//		}
//		break;
//	case Unit::UnitUpgradeType::Damage:
//		if (mGold >= mUnitData[unitType].damage.upgradeCost) {
//			modifyGold(-mUnitData[unitType].damage.upgradeCost);
//			float currentPercentage = mUnitData[unitType].damage.value.current / mUnitData[unitType].damage.value.original * 100.f;
//			mUnitData[unitType].damage.value.current = (currentPercentage + mUnitData[unitType].damage.upgradePercentage) * mUnitData[unitType].damage.value.original / 100.f;
//		}
//		break;
//	case Unit::UnitUpgradeType::Range:
//		if (mGold >= mUnitData[unitType].range.upgradeCost) {
//			modifyGold(-mUnitData[unitType].range.upgradeCost);
//			float currentPercentage = mUnitData[unitType].range.value.current / mUnitData[unitType].range.value.original * 100.f;
//			mUnitData[unitType].range.value.current = (currentPercentage + mUnitData[unitType].range.upgradePercentage) * mUnitData[unitType].range.value.original / 100.f;
//		}
//		break;
//	case Unit::UnitUpgradeType::Rate:
//		if (mGold >= mUnitData[unitType].rate.upgradeCost) {
//			modifyGold(-mUnitData[unitType].rate.upgradeCost);
//			float currentPercentage = mUnitData[unitType].rate.value.current / mUnitData[unitType].rate.value.original * 100.f;
//			mUnitData[unitType].rate.value.current -= sf::seconds((currentPercentage + mUnitData[unitType].rate.upgradePercentage) * mUnitData[unitType].rate.value.original.asSeconds() / 100.f) - mUnitData[unitType].rate.value.current;
//			mUnitData[unitType].rate.value.current = std::max(mUnitData[unitType].rate.value.current, sf::Time::Zero);
//		}
//		break;
//	}
//}
//
//
//void Base::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	HealthEntity::draw(target, states);
//
//	for (const auto& turret : mTurrets)
//		target.draw(turret.first, states);
//
//	for (const auto& unit : mUnits)
//		target.draw(*unit, states);
//
//	target.draw(mSmokeSystem, states.transform *= getTransform());
//	target.draw(mSpawnBar, states);
//}
//
//void Base::drawUnitDamageDisplays(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	for (const auto& unit : mUnits)
//		unit->drawDamageDisplays(target, states);
//
//	for (const auto& turret : mTurrets)
//		turret.first.drawDamageDisplays(target, states);
//}
//
//
