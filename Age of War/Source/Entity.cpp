#include "Entity.h"
#include "DataTables.h"

Entity::Entity(Side side, data::EntityData* entity_data)
	: SpriteNode(*entity_data->texture)
	, side_(side)
	, entity_data_(entity_data)
{
	setOriginFlags(pyro::utils::OriginFlag::Center);
	scale(side_ == Side::Enemy ? -entity_data_->scale : entity_data_->scale,
		  entity_data_->scale);
}

Entity::~Entity()
{
}

Entity::EntityType Entity::getEntityType() const
{
	return entity_data_->entity_type;
}