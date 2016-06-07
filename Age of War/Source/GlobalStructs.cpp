#include "GlobalStructs.h"
#include "Turret.h"
#include "Tools/TextureDataReader.h"

#include <PYRO/Math.h>

namespace gStruct
{
	std::vector<UnitData> initializeUnitData()
	{
		std::vector<UnitData> data(Unit::UnitType::TypeCount);

		data[Unit::Mage].generalUnitType = Unit::GeneralUnitType::Ranged;
		data[Unit::Mage].unitType        = Unit::Mage;
		data[Unit::Mage].name            = "Mage";
		data[Unit::Mage].health          = 75;
		data[Unit::Mage].damage          = 15;
		data[Unit::Mage].range           = 70.f;
		data[Unit::Mage].rate            = sf::seconds(0.7f);
		data[Unit::Mage].speed           = 50.f;
		data[Unit::Mage].spawn           = sf::seconds(2.f);
		data[Unit::Mage].cost            = 25;
		data[Unit::Mage].scale           = 0.5f;
		setupAnimation(data[Unit::Mage], true, true);
		data[Unit::Mage].iconRect = data[Unit::Mage].walkRects.front().first;
		data[Unit::Mage].rangedData = std::unique_ptr<RangedUnitData>(new RangedUnitData());
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
		data[Unit::Knight].health          = 200;
		data[Unit::Knight].damage          = 45;
		data[Unit::Knight].range           = 15.f;
		data[Unit::Knight].rate            = sf::seconds(0.75f);
		data[Unit::Knight].speed           = 65.f;
		data[Unit::Knight].spawn           = sf::seconds(3.5f);
		data[Unit::Knight].cost            = 50;
		data[Unit::Knight].scale           = 0.5f;
		setupAnimation(data[Unit::Knight], true, true);
		data[Unit::Knight].iconRect = data[Unit::Knight].walkRects.front().first;
		data[Unit::Knight].rangedData = nullptr;

		data[Unit::Samurai].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Samurai].unitType = Unit::Samurai;
		data[Unit::Samurai].name = "Samurai";
		data[Unit::Samurai].health = 280;
		data[Unit::Samurai].damage = 25;
		data[Unit::Samurai].range = 15.f;
		data[Unit::Samurai].rate = sf::seconds(0.45f);
		data[Unit::Samurai].speed = 70.f;
		data[Unit::Samurai].spawn = sf::seconds(3.5f);
		data[Unit::Samurai].cost = 150;
		data[Unit::Samurai].scale = 0.25f;
		setupAnimation(data[Unit::Samurai], false, false);
		data[Unit::Samurai].iconRect = data[Unit::Samurai].walkRects.front().first;
		data[Unit::Samurai].rangedData = nullptr;

		data[Unit::Shadow].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Shadow].unitType = Unit::Shadow;
		data[Unit::Shadow].name = "Shadow";
		data[Unit::Shadow].health = 350;
		data[Unit::Shadow].damage = 65;
		data[Unit::Shadow].range = 15.f;
		data[Unit::Shadow].rate = sf::seconds(0.6f);
		data[Unit::Shadow].speed = 90.f;
		data[Unit::Shadow].spawn = sf::seconds(3.f);
		data[Unit::Shadow].cost = 350;
		data[Unit::Shadow].scale = 0.5f;
		setupAnimation(data[Unit::Shadow], false, false);
		data[Unit::Shadow].iconRect = data[Unit::Shadow].walkRects.front().first;
		data[Unit::Shadow].rangedData = nullptr;

		data[Unit::Destroyer].generalUnitType = Unit::GeneralUnitType::Ranged;
		data[Unit::Destroyer].unitType        = Unit::Destroyer;
		data[Unit::Destroyer].name            = "Destroyer";
		data[Unit::Destroyer].health          = 500;
		data[Unit::Destroyer].damage          = 75;
		data[Unit::Destroyer].range           = 60.f;
		data[Unit::Destroyer].rate            = sf::seconds(0.75f);
		data[Unit::Destroyer].speed           = 50.f;
		data[Unit::Destroyer].spawn           = sf::seconds(4.5f);
		data[Unit::Destroyer].cost            = 450;
		data[Unit::Destroyer].scale           = 0.85f;
		setupAnimation(data[Unit::Destroyer], true, true);
		data[Unit::Destroyer].iconRect = data[Unit::Destroyer].walkRects.front().first;
		data[Unit::Destroyer].rangedData = std::unique_ptr<RangedUnitData>(new RangedUnitData());
		data[Unit::Destroyer].rangedData->projectileSpeed = 500.f;
		data[Unit::Destroyer].rangedData->spawnProjectile =
			RangedUnitData::SpawnProjectileFunction([](std::vector<sf::VertexArray>& projs, sf::Vector2f unitSize)
		{
			int i = rand() % 2;
			const sf::Vector2f size(15.f, 5.f);
			const sf::Vector2f startingPos(i == 0 ? 0.f : unitSize.x + size.x, i == 0 ? size.y : 0.f);

			projs.emplace_back(sf::VertexArray(sf::Quads, 4));
			projs.back()[0].position = sf::Vector2f(0.f, 0.f);
			projs.back()[1].position = sf::Vector2f(size.x, 0.f);
			projs.back()[2].position = size;
			projs.back()[3].position = sf::Vector2f(0.f, size.y);
			
			projs.back()[0].color =
			projs.back()[1].color =
			projs.back()[2].color =
			projs.back()[3].color = sf::Color::Red;
		});

		data[Unit::Executioner].generalUnitType = Unit::GeneralUnitType::Melee;
		data[Unit::Executioner].unitType        = Unit::Executioner;
		data[Unit::Executioner].name            = "Executioner";
		data[Unit::Executioner].health          = 600;
		data[Unit::Executioner].damage          = 40;
		data[Unit::Executioner].range           = 15.f;
		data[Unit::Executioner].rate            = sf::seconds(0.35f);
		data[Unit::Executioner].speed           = 55.f;
		data[Unit::Executioner].spawn           = sf::seconds(5.f);
		data[Unit::Executioner].cost            = 550;
		data[Unit::Executioner].scale           = 0.9f;
		setupAnimation(data[Unit::Executioner], true, true);
		data[Unit::Executioner].iconRect = data[Unit::Executioner].attackRects.front().first;
		data[Unit::Executioner].rangedData = nullptr;


		return std::move(data);
	}

	std::vector<TurretData> initializeTurretData()
	{
		std::vector<TurretData> data(Turret::TypeCount);

		data[Turret::LaserTurret].turretType      = Turret::LaserTurret;
		data[Turret::LaserTurret].name            = "Laser Turret";
		data[Turret::LaserTurret].health          = 0;
		data[Turret::LaserTurret].damage          = 8;
		data[Turret::LaserTurret].range           = 220.f;
		data[Turret::LaserTurret].spawn           = sf::seconds(0.f);
		data[Turret::LaserTurret].rate            = sf::seconds(0.75f);
		data[Turret::LaserTurret].cost            = 175;
		data[Turret::LaserTurret].scale           = 0.75f;
		data[Turret::LaserTurret].projectileSpeed = 450.f;
		data[Turret::LaserTurret].iconRect        = sf::IntRect();

		return std::move(data);
	}

	void setupAnimation(UnitData& unitData, bool walkLoopback, bool attackLoopback)
	{
		unitData.walkRects = std::move(readTextureData(unitData.name, "Walk"));
		unitData.attackRects = std::move(readTextureData(unitData.name, "Attack"));

		if (walkLoopback)
			for (int i = unitData.walkRects.size() - 2; i >= 0; i--)
				unitData.walkRects.push_back(unitData.walkRects[i]);
		if (attackLoopback)
			for (int i = unitData.attackRects.size() - 2; i >= 0; i--)
				unitData.attackRects.push_back(unitData.attackRects[i]);
	}
}