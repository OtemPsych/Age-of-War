#ifndef Entity_H_
#define Entity_H_

#include <PYRO/SpriteNode.h>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace data { struct EntityData; }
class Entity : public pyro::SpriteNode
{
public:
	enum class Side { Ally, Enemy };
	enum class EntityType { Unit, Turret, Base };

public:
	Entity(Side side, data::EntityData* entity_data);
	virtual ~Entity();
public:
	EntityType getEntityType() const;

protected:
	Side              side_;
	data::EntityData* entity_data_;
};
#endif