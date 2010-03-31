#ifndef ENTITY_H
#define ENTITY_H

#include "entityType.h"

class IEntity {
public:

    virtual EntityType getEntityType() { return ENTT_NONE; } ;

};

#endif
