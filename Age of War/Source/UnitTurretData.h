#ifndef UnitTurretData_H_
#define UnitTurretData_H_

#include <vector>
#include <functional>
#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <PYRO/Utils.h>

namespace data
{
	class UnitTurretData
	{
	protected:
		template <typename T>
		struct UpgradeSystem 
		{
			pyro::utils::Resource<T> value;
			unsigned short           upgrade_percentage;
			unsigned short           upgrade_cost;
		};
	public:
		std::string             name_;
		UpgradeSystem<float>    damage_;
		UpgradeSystem<float>    range_;
		UpgradeSystem<sf::Time> rate_;
		unsigned short          cost_;

		float                   scale_;
		sf::IntRect             icon_rect_;

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
			using SpawnProjectileFunction = std::function<void(std::vector<sf::VertexArray>&, sf::Vector2f)>;

			float                   projectile_speed;
			SpawnProjectileFunction spawnProjectile;
		};
	public:
		unsigned short                  general_unit_type_;
		unsigned short                  unit_type_;

		UpgradeSystem<float>            health_;
		float                           speed_;
		sf::Time                        spawn_;

		TextureData                     walk_rects_;
		TextureData                     attack_rects_;

		std::unique_ptr<RangedUnitData> ranged_data_;

	private:
		void setupAnimation(bool walk_loopback, bool attack_loopback);
	public:
		static std::vector<UnitData> initializeUnitData();
	};

	class TurretData : public UnitTurretData
	{
	public:
		unsigned short turret_type_;
		float          projectile_speed_;

	public:
		TurretData() = default;
	public:
		static std::vector<TurretData> initializeTurretData();
	};
}
#endif