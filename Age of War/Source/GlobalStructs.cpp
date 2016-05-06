#include "GlobalStructs.h"
#include "Turret.h"

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
		data[Unit::Mage].damage          = 25;
		data[Unit::Mage].range           = 70.f;
		data[Unit::Mage].rate            = sf::seconds(0.7f);
		data[Unit::Mage].speed           = 50.f;
		data[Unit::Mage].spawn           = sf::seconds(2.f);
		data[Unit::Mage].cost            = 25;
		data[Unit::Mage].scale           = 0.25f;
		data[Unit::Mage].walkRects.emplace_back(sf::IntRect(0, 0, 93, 134));
		data[Unit::Mage].walkRects.emplace_back(sf::IntRect(107, 4, 114, 128));
		data[Unit::Mage].attackRects.emplace_back(sf::IntRect(107, 4, 114, 128));
		data[Unit::Mage].attackRects.emplace_back(sf::IntRect(232, 5, 121, 123));
		data[Unit::Mage].attackRects.emplace_back(sf::IntRect(380, 5, 121, 123));
		data[Unit::Mage].rangedData = std::unique_ptr<RangedUnitData>(new RangedUnitData());
		data[Unit::Mage].rangedData->projectileSpeed = 700.f;
		data[Unit::Mage].rangedData->spawnProjectile = 
			RangedUnitData::SpawnProjectileFunction([](std::vector<sf::VertexArray>& projs, sf::Vector2f unitSize)
		{ 
			const float radius = 35.f;
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
		data[Unit::Knight].health          = 250;
		data[Unit::Knight].damage          = 45;
		data[Unit::Knight].range           = 18.f;
		data[Unit::Knight].rate            = sf::seconds(0.75f);
		data[Unit::Knight].speed           = 65.f;
		data[Unit::Knight].spawn           = sf::seconds(3.5f);
		data[Unit::Knight].cost            = 50;
		data[Unit::Knight].scale           = 0.25f;
		data[Unit::Knight].walkRects.emplace_back(sf::IntRect(66, 2, 210, 252));
		data[Unit::Knight].walkRects.emplace_back(sf::IntRect(318, 11, 260, 246));
		data[Unit::Knight].attackRects.emplace_back(sf::IntRect(643, 15, 306, 236));
		data[Unit::Knight].attackRects.emplace_back(sf::IntRect(992, 9, 315, 226));
		data[Unit::Knight].attackRects.emplace_back(sf::IntRect(1335, 10, 303, 260));
		data[Unit::Knight].attackRects.emplace_back(sf::IntRect(1699, 11, 272, 256));
		data[Unit::Knight].rangedData = nullptr;

		data[Unit::Destroyer].generalUnitType = Unit::GeneralUnitType::Ranged;
		data[Unit::Destroyer].unitType        = Unit::Destroyer;
		data[Unit::Destroyer].name            = "Destroyer";
		data[Unit::Destroyer].health          = 500;
		data[Unit::Destroyer].damage          = 75;
		data[Unit::Destroyer].range           = 25.f;
		data[Unit::Destroyer].rate            = sf::seconds(0.75f);
		data[Unit::Destroyer].speed           = 50.f;
		data[Unit::Destroyer].spawn           = sf::seconds(5.f);
		data[Unit::Destroyer].cost            = 250;
		data[Unit::Destroyer].scale           = 0.85f;
		data[Unit::Destroyer].walkRects.emplace_back(sf::IntRect(0, 0, 212, 169));
		data[Unit::Destroyer].walkRects.emplace_back(sf::IntRect(248, 0, 206, 168));
		data[Unit::Destroyer].attackRects.emplace_back(sf::IntRect(484, 9, 171, 160));
		data[Unit::Destroyer].attackRects.emplace_back(sf::IntRect(484, 9, 171, 160));
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

		for (unsigned i = 0; i < data.size(); i++)
		{
			for (int j = data[i].walkRects.size() - 2; j >= 0; j--)
				data[i].walkRects.push_back(data[i].walkRects[j]);

			for (int j = data[i].attackRects.size() - 2; j >= 0; j--)
				data[i].attackRects.push_back(data[i].attackRects[j]);
		}

		return std::move(data);
	}

	std::vector<TurretData> initializeTurretData()
	{
		std::vector<TurretData> data(Turret::TypeCount);

		data[Turret::LaserTurret].turretType      = Turret::LaserTurret;
		data[Turret::LaserTurret].name            = "Laser Turret";
		data[Turret::LaserTurret].damage          = 15;
		data[Turret::LaserTurret].range           = 120.f;
		data[Turret::LaserTurret].rate            = sf::seconds(0.4f);
		data[Turret::LaserTurret].cost            = 150;
		data[Turret::LaserTurret].scale           = 1.f;
		data[Turret::LaserTurret].projectileSpeed = 450.f;

		return std::move(data);
	}
}