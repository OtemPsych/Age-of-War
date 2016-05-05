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

	struct RangedUnitData
	{
		using SpawnProjectileFunction = std::function<void(std::vector<sf::VertexArray>&, sf::Vector2f unitSize)>;

		float                   projectileSpeed;
		SpawnProjectileFunction spawnProjectile;
	};

	struct UnitData
	{
		unsigned short                  generalUnitType;
		unsigned short	                unitType;
		std::string                     name;
		unsigned short			        health;
		unsigned short			        damage;
		float					        range;
		sf::Time				        rate;
		float					        speed;
		sf::Time				        spawn;
		unsigned short			        cost;
		float					        scale;
		std::vector<sf::IntRect>        walkRects;
		std::vector<sf::IntRect>        attackRects;

		std::unique_ptr<RangedUnitData> rangedData;
	};

	std::vector<UnitData> initializeUnitData();
}
#endif