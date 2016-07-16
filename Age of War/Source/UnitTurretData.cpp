#include "UnitTurretData.h"
#include "Turret.h"
#include "Tools/TextureDataReader.h"

#include <PYRO/Math.h>

namespace data
{
	UnitTurretData::~UnitTurretData()
	{
	}

	void UnitData::setupAnimation(bool walk_loopback, bool attack_loopback)
	{
		walk_rects_ = std::move(readTextureData(name_, "Walk"));
		attack_rects_ = std::move(readTextureData(name_, "Attack"));

		if (walk_loopback)
			for (int i = walk_rects_.size() - 2; i >= 0; --i)
				walk_rects_.push_back(walk_rects_[i]);
		if (attack_loopback)
			for (int i = attack_rects_.size() - 2; i >= 0; --i)
				attack_rects_.push_back(attack_rects_[i]);
	}

	std::vector<UnitData> UnitData::initializeUnitData()
	{
		using namespace pyro::utils;

		std::vector<UnitData> data(Unit::UnitType::TypeCount);

		data[Unit::Mage].general_unit_type_         = Unit::GeneralUnitType::Ranged;
		data[Unit::Mage].unit_type_                 = Unit::Mage;
		data[Unit::Mage].name_                      = "Mage";
		data[Unit::Mage].health_.value              = Resource<float>(75.f);
		data[Unit::Mage].health_.upgrade_percentage = 25;
		data[Unit::Mage].health_.upgrade_cost       = 5;
		data[Unit::Mage].damage_.value              = Resource<float>(15.f);
		data[Unit::Mage].damage_.upgrade_percentage = 25;
		data[Unit::Mage].damage_.upgrade_cost       = 0;
		data[Unit::Mage].range_.value               = Resource<float>(95.f);
		data[Unit::Mage].range_.upgrade_percentage  = 25;
		data[Unit::Mage].range_.upgrade_cost        = 5;
		data[Unit::Mage].rate_.value                = Resource<sf::Time>(sf::seconds(0.7f));
		data[Unit::Mage].rate_.upgrade_percentage   = 25;
		data[Unit::Mage].rate_.upgrade_cost         = 5;
		data[Unit::Mage].speed_                     = 50.f;
		data[Unit::Mage].spawn_                     = sf::seconds(2.f);
		data[Unit::Mage].cost_                      = 25;
		data[Unit::Mage].scale_                     = 0.5f;
		data[Unit::Mage].setupAnimation(true, true);
		data[Unit::Mage].icon_rect_ = data[Unit::Mage].walk_rects_.front().first;
		data[Unit::Mage].ranged_data_ = std::make_unique<RangedUnitData>();
		data[Unit::Mage].ranged_data_->projectile_speed = 500.f;
		data[Unit::Mage].ranged_data_->spawnProjectile =
			RangedUnitData::SpawnProjectileFunction([](std::vector<sf::VertexArray>& projs, sf::Vector2f unitSize)
		{
			const float radius = 17.f;
			const sf::Vector2f center(unitSize.x / 2.f + radius, -unitSize.y / 2.f - radius / 2.f);

			projs.emplace_back(sf::VertexArray(sf::TrianglesFan, 32));
			projs.back()[0].position = center;
			projs.back()[0].color = sf::Color::Black;

			for (unsigned i = 1; i < projs.back().getVertexCount(); i++)
			{
				float angle = Math_PI * 2.f * i / (projs.back().getVertexCount() - 2);
				projs.back()[i].position = center + sf::Vector2f(cos(angle) * radius, sin(angle) * radius);
				projs.back()[i].color = i % 5 == 0 ? sf::Color::White : sf::Color::Magenta;
			}
		});

		data[Unit::Knight].general_unit_type_ = Unit::GeneralUnitType::Melee;
		data[Unit::Knight].unit_type_         = Unit::Knight;
		data[Unit::Knight].name_              = "Knight";
		data[Unit::Knight].health_.value      = Resource<float>(200.f);
		data[Unit::Knight].damage_.value      = Resource<float>(35.f);
		data[Unit::Knight].range_.value       = Resource<float>(15.f);
		data[Unit::Knight].rate_.value        = Resource<sf::Time>(sf::seconds(0.75f));
		data[Unit::Knight].speed_             = 65.f;
		data[Unit::Knight].spawn_             = sf::seconds(3.5f);
		data[Unit::Knight].cost_              = 50;
		data[Unit::Knight].scale_             = 0.5f;
		data[Unit::Knight].setupAnimation(true, true);
		data[Unit::Knight].icon_rect_ = data[Unit::Knight].walk_rects_.front().first;
		data[Unit::Knight].ranged_data_.reset();

		data[Unit::Samurai].general_unit_type_ = Unit::GeneralUnitType::Melee;
		data[Unit::Samurai].unit_type_         = Unit::Samurai;
		data[Unit::Samurai].name_              = "Samurai";
		data[Unit::Samurai].health_.value      = Resource<float>(280.f);
		data[Unit::Samurai].damage_.value      = Resource<float>(25.f);
		data[Unit::Samurai].range_.value       = Resource<float>(15.f);
		data[Unit::Samurai].rate_.value        = Resource<sf::Time>(sf::seconds(0.45f));
		data[Unit::Samurai].speed_             = 70.f;
		data[Unit::Samurai].spawn_             = sf::seconds(3.5f);
		data[Unit::Samurai].cost_              = 150;
		data[Unit::Samurai].scale_             = 0.25f;
		data[Unit::Samurai].setupAnimation(false, false);
		data[Unit::Samurai].icon_rect_ = data[Unit::Samurai].walk_rects_.front().first;
		data[Unit::Samurai].ranged_data_.reset();

		data[Unit::Shadow].general_unit_type_ = Unit::GeneralUnitType::Melee;
		data[Unit::Shadow].unit_type_         = Unit::Shadow;
		data[Unit::Shadow].name_              = "Shadow";
		data[Unit::Shadow].health_.value      = Resource<float>(350.f);
		data[Unit::Shadow].damage_.value      = Resource<float>(65.f);
		data[Unit::Shadow].range_.value       = Resource<float>(15.f);
		data[Unit::Shadow].rate_.value        = Resource<sf::Time>(sf::seconds(0.6f));
		data[Unit::Shadow].speed_             = 90.f;
		data[Unit::Shadow].spawn_             = sf::seconds(3.f);
		data[Unit::Shadow].cost_              = 350;
		data[Unit::Shadow].scale_             = 0.5f;
		data[Unit::Shadow].setupAnimation(false, false);
		data[Unit::Shadow].icon_rect_ = data[Unit::Shadow].walk_rects_.front().first;
		data[Unit::Shadow].ranged_data_.reset();

		data[Unit::Destroyer].general_unit_type_ = Unit::GeneralUnitType::Ranged;
		data[Unit::Destroyer].unit_type_         = Unit::Destroyer;
		data[Unit::Destroyer].name_              = "Destroyer";
		data[Unit::Destroyer].health_.value      = Resource<float>(500.f);
		data[Unit::Destroyer].damage_.value      = Resource<float>(50.f);
		data[Unit::Destroyer].range_.value       = Resource<float>(60.f);
		data[Unit::Destroyer].rate_.value        = Resource<sf::Time>(sf::seconds(0.7f));
		data[Unit::Destroyer].speed_             = 50.f;
		data[Unit::Destroyer].spawn_             = sf::seconds(4.5f);
		data[Unit::Destroyer].cost_              = 450;
		data[Unit::Destroyer].scale_             = 0.85f;
		data[Unit::Destroyer].setupAnimation(true, true);
		data[Unit::Destroyer].icon_rect_ = data[Unit::Destroyer].walk_rects_.front().first;
		data[Unit::Destroyer].ranged_data_ = std::make_unique<RangedUnitData>();
		data[Unit::Destroyer].ranged_data_->projectile_speed = 500.f;
		data[Unit::Destroyer].ranged_data_->spawnProjectile =
			RangedUnitData::SpawnProjectileFunction([](std::vector<sf::VertexArray>& projs, sf::Vector2f unitSize)
		{
			const sf::Vector2f size(15.f, 5.f);
			int i = rand() % 2;
			sf::Vector2f startingPos(i == 0 ? sf::Vector2f(size.x, unitSize.y / 4.f + size.y)
				: sf::Vector2f(unitSize.x / 2.f + size.x * 2.f, unitSize.y / 4.f - size.y));

			projs.emplace_back(sf::VertexArray(sf::Quads, 4));
			projs.back()[0].position = startingPos;
			projs.back()[1].position = sf::Vector2f(startingPos.x + size.x, startingPos.y);
			projs.back()[2].position = startingPos + size;
			projs.back()[3].position = sf::Vector2f(startingPos.x, startingPos.y + size.y);

			projs.back()[0].color =
				projs.back()[1].color =
				projs.back()[2].color =
				projs.back()[3].color = sf::Color::Red;
		});

		data[Unit::Executioner].general_unit_type_ = Unit::GeneralUnitType::Melee;
		data[Unit::Executioner].unit_type_         = Unit::Executioner;
		data[Unit::Executioner].name_              = "Executioner";
		data[Unit::Executioner].health_.value      = Resource<float>(600.f);
		data[Unit::Executioner].damage_.value      = Resource<float>(40.f);
		data[Unit::Executioner].range_.value       = Resource<float>(15.f);
		data[Unit::Executioner].rate_.value        = Resource<sf::Time>(sf::seconds(0.35f));
		data[Unit::Executioner].speed_             = 55.f;
		data[Unit::Executioner].spawn_             = sf::seconds(5.f);
		data[Unit::Executioner].cost_              = 550;
		data[Unit::Executioner].scale_             = 0.9f;
		data[Unit::Executioner].setupAnimation(true, true);
		data[Unit::Executioner].icon_rect_ = data[Unit::Executioner].attack_rects_.front().first;
		data[Unit::Executioner].ranged_data_.reset();

		return std::move(data);
	}

	std::vector<TurretData> TurretData::initializeTurretData()
	{
		using namespace pyro::utils;
		
		std::vector<TurretData> data(Turret::TypeCount);
		
		data[Turret::LaserTurret].turret_type_      = Turret::LaserTurret;
		data[Turret::LaserTurret].name_             = "Laser Turret";
		data[Turret::LaserTurret].damage_.value     = Resource<float>(8.f);
		data[Turret::LaserTurret].range_.value      = Resource<float>(380.f);
		data[Turret::LaserTurret].rate_.value       = Resource<sf::Time>(sf::seconds(0.75f));
		data[Turret::LaserTurret].cost_             = 215;
		data[Turret::LaserTurret].scale_            = 0.75f;
		data[Turret::LaserTurret].projectile_speed_ = 450.f;
		data[Turret::LaserTurret].icon_rect_ = sf::IntRect();
		
		return std::move(data);
	}
}