#ifndef HealthEntity_H_
#define HealthEntity_H_

#include "Entity.h"
#include "GUI/Bar.h"

namespace data { struct HealthEntityData; }
class HealthEntity : public Entity
{
public:
	HealthEntity(Side side, data::HealthEntityData* health_entity_data);
	virtual ~HealthEntity();
public:
	void receiveDamage(unsigned damage);
	virtual bool isDestroyed() const override;

protected:
	data::HealthEntityData* health_entity_data_;
	gui::Bar*               health_bar_;
	unsigned                health_;
};
#endif