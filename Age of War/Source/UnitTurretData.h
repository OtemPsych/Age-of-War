#ifndef UnitTurretData_H_
#define UnitTurretData_H_

#include <PYRO/Utils.h>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <functional>
#include <memory>

namespace data
{
	class UnitTurretData
	{
	protected:
		template <typename T>
		struct UpgradeSystem 
		{
			pyro::utils::Resource<T> value;
			unsigned short           upgradePercentage;
			unsigned short           upgradeCost;
		};
	public:
		std::string             name;
		UpgradeSystem<float>    damage;
		UpgradeSystem<float>    range;
		UpgradeSystem<sf::Time> rate;
		unsigned short          cost;

		float                   scale;
		sf::IntRect             iconRect;

	public:
		UnitTurretData() = default;
		virtual ~UnitTurretData();
	};

	class UnitData : public UnitTurretData
	{
	public:
		using TextureData = std::vector<std::pair<sf::IntRect, sf::Vector2f>>;
	public:
		struct RangedUnitData
		{
			using SpawnProjectileFunction = std::function<void(std::vector<sf::VertexArray>&, sf::Vector2f unitSize)>;

			float                   projectileSpeed;
			SpawnProjectileFunction spawnProjectile;
		};
	public:
		unsigned short                  generalUnitType;
		unsigned short                  unitType;

		UpgradeSystem<float>            health;
		float                           speed;
		sf::Time                        spawn;

		TextureData                     walkRects;
		TextureData                     attackRects;

		std::unique_ptr<RangedUnitData> rangedData;

	private:
		void setupAnimation(bool walkLoopback, bool attackLoopback);
	public:
		static std::vector<UnitData> initializeUnitData();
	};

	class TurretData : public UnitTurretData
	{
	public:
		unsigned short turretType;
		float          projectileSpeed;

	public:
		TurretData() = default;
	public:
		static std::vector<TurretData> initializeTurretData();
	};
}
#endif