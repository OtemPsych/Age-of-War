#ifndef DataTables_H_
#define DataTables_H_

#include <functional>
#include <vector>

#include "Base.h"

namespace data
{
	struct AnimationFrame
	{
		sf::FloatRect texture_rect;
		sf::Vector2f  origin;
		sf::Time      frame_duration;
	};

	struct AnimationData
	{
		std::vector<AnimationFrame> frames;
		sf::Time                    total_duration;
		bool                        repeat;
		bool                        loopback;
	};

	struct ValueDisplayData
	{
		sf::Vector2f    starting_velocity;
		sf::Vector2f    acceleration;
		sf::Time        lifetime;
		const sf::Font* font;
	};

	template <typename T>
	struct UpgradeSystem {
		pyro::utils::Resource<T> value;
		unsigned                 upgradePercentage;
		unsigned                 upgradeCost;
	};

	struct EntityData
	{
		Entity::EntityType entity_type;
		const sf::Texture* texture;
		float              scale;
	};

		struct HealthEntityData : public EntityData
		{
			UpgradeSystem<unsigned> health;
		};

			struct BaseData : public HealthEntityData
			{
				Base::BaseType base_type;
				unsigned       max_population;
				unsigned       gold;
			};

		struct UnitTurretData
		{
			enum class StatType { Name, Cost, Health, Damage, Range, Rate, SpawnTime, StatCount };
			enum class UpgradeStatType { Health, Damage, Range, Rate, UpgradeStatCount };

			std::string                  name;
			UpgradeSystem<unsigned>      damage;
			UpgradeSystem<float>         range;
			UpgradeSystem<sf::Time>      rate;
			unsigned                     cost;
			std::vector<StatType>        stat_types;
			std::vector<UpgradeStatType> upgrade_stat_types;

			sf::FloatRect           icon_rect;

			virtual ~UnitTurretData() {}
		};

			struct UnitData : public UnitTurretData, public HealthEntityData
			{
				Unit::GeneralUnitType   general_unit_type;
				Unit::UnitType          unit_type;
				float                   speed;
				sf::Time                spawn_time;

				AnimationData           walk_animation_data;
				AnimationData           attack_animation_data;

				virtual ~UnitData() {}
			};

				struct RangedUnitData : public UnitData
				{
					std::function<void(std::vector<pyro::VertexArrayNode*>*, const sf::FloatRect&,
						               pyro::SceneNode*)> spawn_projectile;
					float                   projectile_speed;
				};

			struct TurretData : public UnitTurretData, public EntityData
			{
				Turret::TurretType turret_type;
				float              projectile_speed;
			};

	using ValueDisplayDataHolder = std::vector<std::unique_ptr<ValueDisplayData>>;
	ValueDisplayDataHolder initValueDisplayData(const sf::Font& font);

	using BaseDataHolder = std::vector<std::unique_ptr<BaseData>>;
	BaseDataHolder initBaseData(const pyro::TextureHolder<Base::BaseType>& textures);

	using UnitDataHolder = std::vector<std::unique_ptr<UnitData>>;
	UnitDataHolder initUnitData(const pyro::TextureHolder<Unit::UnitType>& textures);

	using TurretDataHolder = std::vector<std::unique_ptr<TurretData>>;
	TurretDataHolder initTurretData(const pyro::TextureHolder<Turret::TurretType>& textures);

	void setupUnitAnimations(UnitData* unit_data);
	void setupIndividualAnimation(AnimationData* animation_data);

	template <typename T>
	void setupStatTypes(T* data)
	{
		if (data->entity_type == Entity::EntityType::Unit) {
			data->stat_types.push_back(UnitTurretData::StatType::Name);
			data->stat_types.push_back(UnitTurretData::StatType::Cost);
			data->stat_types.push_back(UnitTurretData::StatType::Health);
			data->stat_types.push_back(UnitTurretData::StatType::Damage);
			data->stat_types.push_back(UnitTurretData::StatType::Range);
			data->stat_types.push_back(UnitTurretData::StatType::Rate);
			data->stat_types.push_back(UnitTurretData::StatType::SpawnTime);

			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Health);
			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Damage);
			if (dynamic_cast<data::UnitData*>(data)->general_unit_type == Unit::GeneralUnitType::Ranged)
				data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Range);
			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Rate);
		}
		else if (data->entity_type == Entity::EntityType::Turret) {
			data->stat_types.push_back(UnitTurretData::StatType::Name);
			data->stat_types.push_back(UnitTurretData::StatType::Cost);
			data->stat_types.push_back(UnitTurretData::StatType::Damage);
			data->stat_types.push_back(UnitTurretData::StatType::Range);
			data->stat_types.push_back(UnitTurretData::StatType::Rate);

			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Damage);
			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Range);
			data->upgrade_stat_types.push_back(UnitTurretData::UpgradeStatType::Rate);
		}
	}
}
#endif