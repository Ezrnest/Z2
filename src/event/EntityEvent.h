//
// Created by liyicheng on 2019/7/26.
//

#ifndef Z2_ENTITYEVENT_H
#define Z2_ENTITYEVENT_H


#include <world/Point.h>
#include "GameEvent.h"

namespace z2 {

enum class EntityEventType{
    EntityCreated,
    EntityDied,
    ///class EEntityMoved
    EntityMoved,
    EntityAttacked,
    ///class EEntityDamaged
    EntityDamaged,
};

/**
 * Describes an event about an entity.
 */
class EntityEvent : public InGameEvent {
private:
    int entityId;
    EntityEventType entityEventType;
public:
    explicit EntityEvent(int entityId, EntityEventType type);

    int getEntityId() const;
};

class EEntityMoved : public EntityEvent{
private:
    Point from;
    Point dest;
public:
    EEntityMoved(int entityId, Point from, Point dest);

    const Point &getFrom() const;

    const Point &getDest() const;
};

class EEntityDamaged : public EntityEvent{
private:
    int damage;
public:
    EEntityDamaged(int entityId, int damage);

    int getDamage() const;

    void setDamage(int damage);
};



}


#endif //Z2_ENTITYEVENT_H
