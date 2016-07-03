#ifndef GlobalStructs_H_
#define GlobalStructs_H_

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <memory>
#include <functional>

namespace gStruct
{
	template <typename T>
	struct Resource 
	{
		const T original;
		T		current;

		explicit Resource(T amount)
			: original(amount)
			, current(amount)
		{
		}
	};

	using TextureData = std::vector<std::pair<sf::IntRect, sf::Vector2f>>;

	struct RangedUnitData
	{
		using SpawnProjectileFunction = std::function<void(std::vector<sf::VertexArray>&, sf::Vector2f unitSize)>;

		float                   projectileSpeed;
		SpawnProjectileFunction spawnProjectile;
	};
	struct UnitData
	{
		unsigned short generalUnitType;
		unsigned short unitType;
		std::string    name;

		unsigned short health;
		unsigned short healthUpgradePercentage;
		unsigned short healthUpgradeCost;

		unsigned short damage;
		unsigned short damageUpgradePercentage;
		unsigned short damageUpgradeCost;

		float		   range;
		unsigned short rangeUpgradePercentage;
		unsigned short rangeUpgradeCost;

		sf::Time	   rate;
		unsigned short rateUpgradePercentage;
		unsigned short rateUpgradeCost;

		float		   speed;
		sf::Time	   spawn;
		unsigned short cost;
		float		   scale;

		TextureData    walkRects;
		TextureData    attackRects;
		sf::IntRect    iconRect;

		std::unique_ptr<RangedUnitData> rangedData;
	};

	struct TurretData
	{
		unsigned short turretType;
		std::string    name;
		unsigned short health;

		unsigned short damage;
		unsigned short damageUpgradePercentage;
		unsigned short damageUpgradeCost;

		float          range;
		unsigned short rangeUpgradePercentage;
		unsigned short rangeUpgradeCost;

		sf::Time       rate;
		unsigned short rateUpgradePercentage;
		unsigned short rateUpgradeCost;

		sf::Time	   spawn;
		unsigned short cost;
		float          scale;
		float          projectileSpeed;

		sf::IntRect    iconRect;
	};

	std::vector<UnitData> initializeUnitData();
	std::vector<TurretData> initializeTurretData();

	void setupAnimation(UnitData& unitData, bool walkLoopback, bool attackLoopback);
}
#endif