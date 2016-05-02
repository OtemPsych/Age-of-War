#include "GlobalStructs.h"
#include "Unit.h"

#include <PYRO/Math.h>

namespace gStruct
{
	std::vector<UnitData> initializeUnitData()
	{
		std::vector<UnitData> data(Unit::TypeCount);

		data[Unit::Mage].type   = Unit::Mage;
		data[Unit::Mage].name   = "Mage";
		data[Unit::Mage].health = 75;
		data[Unit::Mage].damage = 25;
		data[Unit::Mage].range  = 70.f;
		data[Unit::Mage].rate   = sf::seconds(0.7f);
		data[Unit::Mage].speed  = 50.f;
		data[Unit::Mage].spawn  = sf::seconds(2.f);
		data[Unit::Mage].cost   = 25;
		data[Unit::Mage].scale  = 0.25f;
		data[Unit::Mage].walkRects.push_back(sf::IntRect(0, 0, 93, 134));
		data[Unit::Mage].walkRects.push_back(sf::IntRect(107, 4, 114, 128));
		data[Unit::Mage].attackRects.push_back(sf::IntRect(107, 4, 114, 128));
		data[Unit::Mage].attackRects.push_back(sf::IntRect(232, 5, 121, 123));
		data[Unit::Mage].attackRects.push_back(sf::IntRect(380, 5, 121, 123));

		data[Unit::Knight].type   = Unit::Knight;
		data[Unit::Knight].name   = "Knight";
		data[Unit::Knight].health = 250;
		data[Unit::Knight].damage = 45;
		data[Unit::Knight].range  = 18.f;
		data[Unit::Knight].rate   = sf::seconds(0.75f);
		data[Unit::Knight].speed  = 65.f;
		data[Unit::Knight].spawn  = sf::seconds(3.5f);
		data[Unit::Knight].cost   = 50;
		data[Unit::Knight].scale  = 0.25f;
		data[Unit::Knight].walkRects.push_back(sf::IntRect(66, 2, 210, 252));
		data[Unit::Knight].walkRects.push_back(sf::IntRect(318, 11, 260, 246));
		data[Unit::Knight].attackRects.push_back(sf::IntRect(643, 15, 306, 236));
		data[Unit::Knight].attackRects.push_back(sf::IntRect(992, 9, 315, 226));
		data[Unit::Knight].attackRects.push_back(sf::IntRect(1335, 10, 303, 260));
		data[Unit::Knight].attackRects.push_back(sf::IntRect(1699, 11, 272, 256));

		data[Unit::Destroyer].type   = Unit::Destroyer;
		data[Unit::Destroyer].name   = "Destroyer";
		data[Unit::Destroyer].health = 500;
		data[Unit::Destroyer].damage = 75;
		data[Unit::Destroyer].range  = 25.f;
		data[Unit::Destroyer].rate   = sf::seconds(0.75f);
		data[Unit::Destroyer].speed  = 50.f;
		data[Unit::Destroyer].spawn  = sf::seconds(5.f);
		data[Unit::Destroyer].cost   = 250;
		data[Unit::Destroyer].scale  = 0.85f;
		data[Unit::Destroyer].walkRects.push_back(sf::IntRect(4, 15, 211, 169));
		data[Unit::Destroyer].walkRects.push_back(sf::IntRect(287, 10, 207, 167));
		data[Unit::Destroyer].attackRects.push_back(sf::IntRect(874, 0, 172, 157));
		data[Unit::Destroyer].attackRects.push_back(sf::IntRect(874, 0, 172, 157));

		for (unsigned i = 0; i < data.size(); i++)
		{
			for (int j = data[i].walkRects.size() - 2; j >= 0; j--)
				data[i].walkRects.push_back(data[i].walkRects[j]);

			for (int j = data[i].attackRects.size() - 2; j >= 0; j--)
				data[i].attackRects.push_back(data[i].attackRects[j]);
		}

		return std::move(data);
	}
}