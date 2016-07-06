#include "UnitTurretData.h"
#include "Turret.h"
#include "Tools/TextureDataReader.h"

#include <PYRO/Math.h>

namespace data
{
	UnitTurretData::~UnitTurretData()
	{
	}

	void UnitData::setupAnimation(bool walkLoopback, bool attackLoopback)
	{
		walkRects = std::move(readTextureData(name, "Walk"));
		attackRects = std::move(readTextureData(name, "Attack"));

		if (walkLoopback)
			for (int i = walkRects.size() - 2; i >= 0; i--)
				walkRects.push_back(walkRects[i]);
		if (attackLoopback)
			for (int i = attackRects.size() - 2; i >= 0; i--)
				attackRects.push_back(attackRects[i]);
	}

	std::vector<UnitData> UnitData::initializeUnitData()
	{
		using namespace pyro::utils;

		std::vector<UnitData> data(Unit::UnitType::TypeCount);

		data[Unit::Mage].generalUnitType          = Unit::GeneralUnitType::Ranged;
		data[Unit::Mage].unitType                 = Unit::Mage;
		data[Unit::Mage].name                     = "Mage";
		data[Unit::Mage].health.value             = Resource<float>(75.f);
		data[Unit::Mage].health.upgradePercentage = 25;
		data[Unit::Mage].health.upgradeCost       = 50;
		data[Unit::Mage].damage.value             = Resource<float>(15.f);
		data[Unit::Mage].damage.upgradePercentage = 25;
		data[Unit::Mage].damage.upgradeCost       = 50;
		data[Unit::Mage].range.value              = Resource<float>(95.f);
		data[Unit::Mage].range.upgradePercentage  = 25;
		data[Unit::Mage].range.upgradeCost        = 50;
		data[Unit::Mage].rate.value               = Resource<sf::Time>(sf::seconds(0.7f));
		data[Unit::Mage].rate.upgradePercentage   = 25;
		data[Unit::Mage].rate.upgradeCost         = 5;
		data[Unit::Mage].speed                    = 50.f;
		data[Unit::Mage].spawn                    = sf::seconds(2.f);
		data[Unit::Mage].cost                     = 25;
		data[Unit::Mage].scale                    = 0.5f;
		data[Unit::Mage].setupAnimation(true, true);
		data[Unit::Mage].iconRect = data[Unit::Mage].walkRects.front().first;
		data[Unit::Mage].rangedData = std::make_unique<RangedUnitData>();
		data[Unit::Mage].rangedData->projectileSpeed = 500.f;
		data[Unit::Mage].rangedData->spawnProjectile =
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

		data[Unit::Knight].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Knight].unitType        = Unit::Knight;
		data[Unit::Knight].name            = "Knight";
		data[Unit::Knight].health.value    = Resource<float>(200.f);
		data[Unit::Knight].damage.value    = Resource<float>(35.f);
		data[Unit::Knight].range.value     = Resource<float>(15.f);
		data[Unit::Knight].rate.value      = Resource<sf::Time>(sf::seconds(0.75f));
		data[Unit::Knight].speed           = 65.f;
		data[Unit::Knight].spawn           = sf::seconds(3.5f);
		data[Unit::Knight].cost            = 50;
		data[Unit::Knight].scale           = 0.5f;
		data[Unit::Knight].setupAnimation(true, true);
		data[Unit::Knight].iconRect = data[Unit::Knight].walkRects.front().first;
		data[Unit::Knight].rangedData.reset();

		data[Unit::Samurai].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Samurai].unitType        = Unit::Samurai;
		data[Unit::Samurai].name            = "Samurai";
		data[Unit::Samurai].health.value    = Resource<float>(280.f);
		data[Unit::Samurai].damage.value    = Resource<float>(25.f);
		data[Unit::Samurai].range.value     = Resource<float>(15.f);
		data[Unit::Samurai].rate.value      = Resource<sf::Time>(sf::seconds(0.45f));
		data[Unit::Samurai].speed           = 70.f;
		data[Unit::Samurai].spawn           = sf::seconds(3.5f);
		data[Unit::Samurai].cost            = 150;
		data[Unit::Samurai].scale           = 0.25f;
		data[Unit::Samurai].setupAnimation(false, false);
		data[Unit::Samurai].iconRect = data[Unit::Samurai].walkRects.front().first;
		data[Unit::Samurai].rangedData.reset();

		data[Unit::Shadow].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Shadow].unitType        = Unit::Shadow;
		data[Unit::Shadow].name            = "Shadow";
		data[Unit::Shadow].health.value    = Resource<float>(350.f);
		data[Unit::Shadow].damage.value    = Resource<float>(65.f);
		data[Unit::Shadow].range.value     = Resource<float>(15.f);
		data[Unit::Shadow].rate.value      = Resource<sf::Time>(sf::seconds(0.6f));
		data[Unit::Shadow].speed           = 90.f;
		data[Unit::Shadow].spawn           = sf::seconds(3.f);
		data[Unit::Shadow].cost            = 350;
		data[Unit::Shadow].scale           = 0.5f;
		data[Unit::Shadow].setupAnimation(false, false);
		data[Unit::Shadow].iconRect = data[Unit::Shadow].walkRects.front().first;
		data[Unit::Shadow].rangedData.reset();

		data[Unit::Destroyer].generalUnitType = Unit::GeneralUnitType::Ranged;
		data[Unit::Destroyer].unitType        = Unit::Destroyer;
		data[Unit::Destroyer].name            = "Destroyer";
		data[Unit::Destroyer].health.value    = Resource<float>(500.f);
		data[Unit::Destroyer].damage.value    = Resource<float>(50.f);
		data[Unit::Destroyer].range.value     = Resource<float>(60.f);
		data[Unit::Destroyer].rate.value      = Resource<sf::Time>(sf::seconds(0.7f));
		data[Unit::Destroyer].speed           = 50.f;
		data[Unit::Destroyer].spawn           = sf::seconds(4.5f);
		data[Unit::Destroyer].cost            = 450;
		data[Unit::Destroyer].scale           = 0.85f;
		data[Unit::Destroyer].setupAnimation(true, true);
		data[Unit::Destroyer].iconRect = data[Unit::Destroyer].walkRects.front().first;
		data[Unit::Destroyer].rangedData = std::make_unique<RangedUnitData>();
		data[Unit::Destroyer].rangedData->projectileSpeed = 500.f;
		data[Unit::Destroyer].rangedData->spawnProjectile =
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

		data[Unit::Executioner].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Executioner].unitType        = Unit::Executioner;
		data[Unit::Executioner].name            = "Executioner";
		data[Unit::Executioner].health.value    = Resource<float>(600.f);
		data[Unit::Executioner].damage.value    = Resource<float>(40.f);
		data[Unit::Executioner].range.value     = Resource<float>(15.f);
		data[Unit::Executioner].rate.value      = Resource<sf::Time>(sf::seconds(0.35f));
		data[Unit::Executioner].speed           = 55.f;
		data[Unit::Executioner].spawn           = sf::seconds(5.f);
		data[Unit::Executioner].cost            = 550;
		data[Unit::Executioner].scale           = 0.9f;
		data[Unit::Executioner].setupAnimation(true, true);
		data[Unit::Executioner].iconRect = data[Unit::Executioner].attackRects.front().first;
		data[Unit::Executioner].rangedData.reset();

		return std::move(data);
	}

	std::vector<TurretData> TurretData::initializeTurretData()
	{
		using namespace pyro::utils;
		
		std::vector<TurretData> data(Turret::TypeCount);
		
		data[Turret::LaserTurret].turretType      = Turret::LaserTurret;
		data[Turret::LaserTurret].name            = "Laser Turret";
		data[Turret::LaserTurret].damage.value    = Resource<float>(8.f);
		data[Turret::LaserTurret].range.value     = Resource<float>(380.f);
		data[Turret::LaserTurret].rate.value      = Resource<sf::Time>(sf::seconds(0.75f));
		data[Turret::LaserTurret].cost            = 215;
		data[Turret::LaserTurret].scale           = 0.75f;
		data[Turret::LaserTurret].projectileSpeed = 450.f;
		data[Turret::LaserTurret].iconRect        = sf::IntRect();
		
		return std::move(data);
	}
}