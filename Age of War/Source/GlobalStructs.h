#ifndef GlobalStructs_H_
#define GlobalStructs_H_

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>

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

	struct UnitData
	{
		unsigned short			 type;
		unsigned short			 health;
		unsigned short			 damage;
		float					 range;
		sf::Time				 rate;
		float					 speed;
		sf::Time				 spawn;
		unsigned short			 cost;
		float					 scale;
		std::vector<sf::IntRect> walkRects;
		std::vector<sf::IntRect> attackRects;
	};

	std::vector<UnitData> initializeUnitData();
}
#endif