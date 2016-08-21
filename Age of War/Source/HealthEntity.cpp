#include "HealthEntity.h"
#include "DataTables.h"

HealthEntity::HealthEntity(Side side, data::HealthEntityData* health_entity_data)
	: Entity(side, health_entity_data)
	, health_entity_data_(health_entity_data)
	, health_bar_(nullptr)
	, health_(health_entity_data->health.value.current)
{
	auto health_bar(std::make_unique<gui::Bar>(getLocalBounds(), side == Side::Ally ? sf::Color(0, 230, 0) : sf::Color(179, 0, 0),
		            entity_data_->entity_type == EntityType::Unit));

	if (side == Side::Enemy && health_entity_data->entity_type == Entity::EntityType::Unit) {
		health_bar->rotate(180.f);
	}

	health_bar_ = health_bar.get();
	attachChild(std::move(health_bar));
}

HealthEntity::~HealthEntity()
{
}

void HealthEntity::receiveDamage(unsigned damage)
{
	if (health_ <= damage)
		health_ = 0;
	else
		health_ -= damage;

	health_bar_->updateBarValue(health_, health_entity_data_->health.value.current);
}

bool HealthEntity::isDestroyed() const
{
	return health_ == 0;
}