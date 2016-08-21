#include "DataTables.h"
#include "Turret.h"
#include "Tools/TextureDataReader.h"

#include <PYRO/Math.h>

namespace data
{
	ValueDisplayDataHolder initValueDisplayData(const sf::Font& font)
	{
		ValueDisplayDataHolder data;
		// Damage
		auto damage_display(std::make_unique<ValueDisplayData>());
		damage_display->starting_velocity = sf::Vector2f(0.f, 42.f);
		damage_display->acceleration      = sf::Vector2f(0.f, -1.7f);
		damage_display->lifetime          = sf::seconds(0.85f);
		damage_display->font              = &font;
		data.push_back(std::move(damage_display));

		return std::move(data);
	}

	BaseDataHolder initBaseData(const pyro::TextureHolder<Base::BaseType>& textures)
	{
		using pyro::utils::Resource;

		BaseDataHolder data;
		// Default Base
		auto default_base_data(std::make_unique<BaseData>());
		default_base_data->entity_type    = Entity::EntityType::Base;
		default_base_data->base_type      = Base::BaseType::DefaultBase;
		default_base_data->health.value   = Resource<unsigned>(2000);
		default_base_data->max_population = 10;
		default_base_data->gold           = 200;
		default_base_data->texture        = &textures.get(default_base_data->base_type);
		default_base_data->scale          = 1.f;
		data.push_back(std::move(default_base_data));

		return std::move(data);
	}

	UnitDataHolder initUnitData(const pyro::TextureHolder<Unit::UnitType>& textures)
	{
		using pyro::utils::Resource;

		UnitDataHolder data;
		// Mage
		auto mage_data(std::make_unique<RangedUnitData>());
		mage_data->entity_type                          = Entity::EntityType::Unit;
		mage_data->general_unit_type                    = Unit::GeneralUnitType::Ranged;
		mage_data->unit_type                            = Unit::UnitType::Mage;
		mage_data->name                                 = "Mage";
		mage_data->health.value                         = Resource<unsigned>(75);
		mage_data->health.upgradePercentage             = 25;
		mage_data->health.upgradeCost                   = 5;
		mage_data->damage.value                         = Resource<unsigned>(15);
		mage_data->damage.upgradePercentage             = 25;
		mage_data->damage.upgradeCost                   = 50;
		mage_data->range.value                          = Resource<float>(95.f);
		mage_data->range.upgradePercentage              = 25;
		mage_data->range.upgradeCost                    = 5;
		mage_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.7f));
		mage_data->rate.upgradePercentage               = 25;
		mage_data->rate.upgradeCost                     = 5;
		mage_data->speed                                = 50.f;
		mage_data->spawn_time                           = sf::seconds(2.f);
		mage_data->cost                                 = 25;
		mage_data->texture                              = &textures.get(mage_data->unit_type);
		mage_data->scale                                = 0.5f;
		mage_data->walk_animation_data.repeat           = true;
		mage_data->walk_animation_data.total_duration   = sf::seconds(0.65f);
		mage_data->walk_animation_data.loopback         = false;
		mage_data->attack_animation_data.repeat         = false;
		mage_data->attack_animation_data.total_duration = mage_data->rate.value.current;
		mage_data->attack_animation_data.loopback       = true ;
		setupUnitAnimations(mage_data.get());
		setupStatTypes(mage_data.get());
		mage_data->icon_rect                            = sf::FloatRect(5.f, 126.f, 30.f, 30.f);
		//mage_data->icon_rect                            = mage_data->walk_animation_data.frames.front().texture_rect;
		mage_data->projectile_speed                     = 320.f;
		mage_data->spawn_projectile = [](std::vector<pyro::VertexArrayNode*>* projs, const sf::FloatRect& gbounds,
			                             pyro::SceneNode* owner)
		{
			const float radius = 8.f;
			const sf::Vector2f center(gbounds.left + gbounds.width - radius, gbounds.top + radius);

			auto projectile(std::make_unique<pyro::VertexArrayNode>(sf::TrianglesFan, 32U));
			projectile->vertices_[0].position = sf::Vector2f(0.f, 0.f);
			projectile->vertices_[0].color = sf::Color::Black;

			for (size_t i = 1; i < projectile->vertices_.getVertexCount(); ++i) {
				float angle = Math_PI * 2.f * i / (projectile->vertices_.getVertexCount() - 2);
				projectile->vertices_[i].position = sf::Vector2f(cos(angle) * radius, sin(angle) * radius);
				projectile->vertices_[i].color = i % 5 == 0 ? sf::Color::White : sf::Color::Magenta;
			}

			projectile->setOriginFlags(pyro::utils::OriginFlag::Center);
			projectile->setPosition(sf::Vector2f(gbounds.left + gbounds.width + radius, gbounds.top + radius));

			projs->push_back(projectile.get());
			owner->attachChild(std::move(projectile));
		};
		data.push_back(std::move(mage_data));

		// Knight
		auto knight_data(std::make_unique<UnitData>());
		knight_data->entity_type                          = Entity::EntityType::Unit;
		knight_data->general_unit_type                    = Unit::GeneralUnitType::Melee;
		knight_data->unit_type                            = Unit::UnitType::Knight;
		knight_data->name                                 = "Knight";
		knight_data->health.value                         = Resource<unsigned>(200);
		knight_data->damage.value                         = Resource<unsigned>(35);
		knight_data->range.value                          = Resource<float>(15.f);
		knight_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.75f));
		knight_data->speed                                = 65.f;
		knight_data->spawn_time                           = sf::seconds(3.5f);
		knight_data->cost                                 = 50;
		knight_data->texture                              = &textures.get(knight_data->unit_type);
		knight_data->scale                                = 0.5f;
		knight_data->walk_animation_data.repeat           = true;
		knight_data->walk_animation_data.total_duration   = sf::seconds(0.65f);
		knight_data->walk_animation_data.loopback         = false;
		knight_data->attack_animation_data.repeat         = false;
		knight_data->attack_animation_data.total_duration = knight_data->rate.value.current;
		knight_data->attack_animation_data.loopback       = true;
		setupUnitAnimations(knight_data.get());
		setupStatTypes(knight_data.get());
		knight_data->icon_rect = sf::FloatRect(15.f, 130.f, 55.f, 55.f);
		//knight_data->icon_rect                            = knight_data->walk_animation_data.frames.front().texture_rect;
		data.push_back(std::move(knight_data));

		// Samurai
		auto samurai_data(std::make_unique<UnitData>());
		samurai_data->entity_type                          = Entity::EntityType::Unit;
		samurai_data->general_unit_type                    = Unit::GeneralUnitType::Melee;
		samurai_data->unit_type                            = Unit::UnitType::Samurai;
		samurai_data->name                                 = "Samurai";
		samurai_data->health.value                         = Resource<unsigned>(280);
		samurai_data->damage.value                         = Resource<unsigned>(25);
		samurai_data->range.value                          = Resource<float>(15.f);
		samurai_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.45f));
		samurai_data->speed                                = 70.f;
		samurai_data->spawn_time                           = sf::seconds(3.5f);
		samurai_data->cost                                 = 150;
		samurai_data->texture                              = &textures.get(samurai_data->unit_type);
		samurai_data->scale                                = 0.25f;
		samurai_data->walk_animation_data.repeat           = true;
		samurai_data->walk_animation_data.total_duration   = sf::seconds(0.65f);
		samurai_data->walk_animation_data.loopback         = false;
		samurai_data->attack_animation_data.repeat         = false;
		samurai_data->attack_animation_data.total_duration = samurai_data->rate.value.current;
		samurai_data->attack_animation_data.loopback       = false;
		setupUnitAnimations(samurai_data.get());
		setupStatTypes(samurai_data.get());
		samurai_data->icon_rect = sf::FloatRect(82.f, 305.f, 57.f, 57.f);
		//samurai_data->icon_rect                            = samurai_data->walk_animation_data.frames.front().texture_rect;
		data.push_back(std::move(samurai_data));

		// Shadow
		auto shadow_data(std::make_unique<UnitData>());
		shadow_data->entity_type                          = Entity::EntityType::Unit;
		shadow_data->general_unit_type                    = Unit::GeneralUnitType::Melee;
		shadow_data->unit_type                            = Unit::UnitType::Shadow;
		shadow_data->name                                 = "Shadow";
		shadow_data->health.value                         = Resource<unsigned>(350);
		shadow_data->damage.value                         = Resource<unsigned>(65);
		shadow_data->range.value                          = Resource<float>(15.f);
		shadow_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.6f));
		shadow_data->speed                                = 90.f;
		shadow_data->spawn_time                           = sf::seconds(3.f);
		shadow_data->cost                                 = 350;
		shadow_data->texture                              = &textures.get(shadow_data->unit_type);
		shadow_data->scale                                = 0.5f;
		shadow_data->walk_animation_data.repeat           = true;
		shadow_data->walk_animation_data.total_duration   = sf::seconds(0.65f);
		shadow_data->walk_animation_data.loopback         = false;
		shadow_data->attack_animation_data.repeat         = false;
		shadow_data->attack_animation_data.total_duration = shadow_data->rate.value.current;
		shadow_data->attack_animation_data.loopback       = false;
		setupUnitAnimations(shadow_data.get());
		setupStatTypes(shadow_data.get());
		shadow_data->icon_rect = sf::FloatRect(432.f, 416.f, 58.f, 58.f);
		//shadow_data->icon_rect                            = shadow_data->walk_animation_data.frames.front().texture_rect;
		data.push_back(std::move(shadow_data));

		// Destroyer
		auto destroyer_data(std::make_unique<RangedUnitData>());
		destroyer_data->entity_type                          = Entity::EntityType::Unit;
		destroyer_data->general_unit_type                    = Unit::GeneralUnitType::Ranged;
		destroyer_data->unit_type                            = Unit::UnitType::Destroyer;
		destroyer_data->name                                 = "Destroyer";
		destroyer_data->health.value                         = Resource<unsigned>(500);
		destroyer_data->damage.value                         = Resource<unsigned>(50);
		destroyer_data->range.value                          = Resource<float>(60.f);
		destroyer_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.7f));
		destroyer_data->speed                                = 50.f;
		destroyer_data->spawn_time                           = sf::seconds(4.5f);
		destroyer_data->cost                                 = 450;
		destroyer_data->texture                              = &textures.get(destroyer_data->unit_type);
		destroyer_data->scale                                = 0.85f;
		destroyer_data->walk_animation_data.repeat           = true;
		destroyer_data->walk_animation_data.total_duration   = sf::seconds(0.45f);
		destroyer_data->walk_animation_data.loopback         = false;
		destroyer_data->attack_animation_data.repeat         = false;
		destroyer_data->attack_animation_data.total_duration = destroyer_data->rate.value.current;
		destroyer_data->attack_animation_data.loopback       = false;
		setupUnitAnimations(destroyer_data.get());
		setupStatTypes(destroyer_data.get());
		destroyer_data->icon_rect = sf::FloatRect(71.f, 180.f, 69.f, 69.f);
		//destroyer_data->icon_rect                            = destroyer_data->walk_animation_data.frames.front().texture_rect;
		destroyer_data->projectile_speed                     = 500.f;
		destroyer_data->spawn_projectile = [](std::vector<pyro::VertexArrayNode*>* projs, const sf::FloatRect& lbounds,
			                                  pyro::SceneNode* owner)
		{
			const sf::Vector2f size(15.f, 5.f);
			int i = rand() % 2;
			//const sf::FloatRect owner_lbounds(owner->getLocalBounds());
			sf::Vector2f startingPos(i == 0 ? sf::Vector2f(size.x, lbounds.height / 4.f + size.y)
				                            : sf::Vector2f(lbounds.width / 2.f + size.x * 2.f,
				                                           lbounds.height / 4.f - size.y));

			auto projectile(std::make_unique<pyro::VertexArrayNode>(size));
			projectile->setFillColor(sf::Color::Red);
			projectile->move(startingPos);

			projs->push_back(projectile.get());
			owner->attachChild(std::move(projectile));
		};
		data.push_back(std::move(destroyer_data));

		// Executioner
		auto executioner_data(std::make_unique<UnitData>());
		executioner_data->entity_type                          = Entity::EntityType::Unit;
		executioner_data->general_unit_type                    = Unit::GeneralUnitType::Melee;
		executioner_data->unit_type                            = Unit::UnitType::Executioner;
		executioner_data->name                                 = "Executioner";
		executioner_data->health.value                         = Resource<unsigned>(600);
		executioner_data->damage.value                         = Resource<unsigned>(40);
		executioner_data->range.value                          = Resource<float>(15.f);
		executioner_data->rate.value                           = Resource<sf::Time>(sf::seconds(0.35f));
		executioner_data->speed                                = 55.f;
		executioner_data->spawn_time                           = sf::seconds(5.f);
		executioner_data->cost                                 = 550;
		executioner_data->texture                              = &textures.get(executioner_data->unit_type);
		executioner_data->scale                                = 0.9f;
		executioner_data->walk_animation_data.repeat           = true;
		executioner_data->walk_animation_data.total_duration   = sf::seconds(0.65f);
		executioner_data->walk_animation_data.loopback         = false;
		executioner_data->attack_animation_data.repeat         = false;
		executioner_data->attack_animation_data.total_duration = executioner_data->rate.value.current;
		executioner_data->attack_animation_data.loopback       = false;
		setupUnitAnimations(executioner_data.get());
		setupStatTypes(executioner_data.get());
		executioner_data->icon_rect = sf::FloatRect(65.f, 24.f, 54.f, 54.f);
		//executioner_data->icon_rect                            = executioner_data->attack_animation_data.frames.front().texture_rect;
		data.push_back(std::move(executioner_data));

		return std::move(data);
	}

	TurretDataHolder initTurretData(const pyro::TextureHolder<Turret::TurretType>& textures)
	{
		using pyro::utils::Resource;

		TurretDataHolder data;
		// Laser Turret
		auto laser_turret_data(std::make_unique<TurretData>());
		laser_turret_data->entity_type      = Entity::EntityType::Turret;
		laser_turret_data->turret_type      = Turret::TurretType::LaserTurret;
		laser_turret_data->name             = "Laser Turret";
		laser_turret_data->damage.value     = Resource<unsigned>(8);
		laser_turret_data->range.value      = Resource<float>(380.f);
		laser_turret_data->rate.value       = Resource<sf::Time>(sf::seconds(0.75f));
		laser_turret_data->cost             = 215;
		laser_turret_data->texture          = &textures.get(laser_turret_data->turret_type);
		laser_turret_data->scale            = 0.75f;
		laser_turret_data->projectile_speed = 450.f;
		setupStatTypes(laser_turret_data.get());
		laser_turret_data->icon_rect        = sf::FloatRect();
		data.push_back(std::move(laser_turret_data));

		return std::move(data);
	}

	void setupUnitAnimations(UnitData* unit_data)
	{
		unit_data->walk_animation_data.frames = std::move(readTextureData(unit_data->name, "Walk"));
		unit_data->attack_animation_data.frames = std::move(readTextureData(unit_data->name, "Attack"));

		setupIndividualAnimation(&unit_data->walk_animation_data);
		setupIndividualAnimation(&unit_data->attack_animation_data);
	}

	void setupIndividualAnimation(AnimationData* animation_data)
	{
		// Loopback Animation (if needed)
		if (animation_data->loopback)
			for (int i = animation_data->frames.size() - 2; i >= 0; --i)
				animation_data->frames.push_back(animation_data->frames[i]);

		// Calculate Time Per Frame
		const sf::Time frame_duration(sf::seconds(animation_data->total_duration.asSeconds() 
			                                     / animation_data->frames.size()));
		for (auto& frame : animation_data->frames)
			frame.frame_duration = frame_duration;
	}
}