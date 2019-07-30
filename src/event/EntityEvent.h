//
// Created by liyicheng on 2019/7/26.
//

#ifndef Z2_ENTITYEVENT_H
#define Z2_ENTITYEVENT_H


#include <world/Point.h>
#include "GameEvent.h"

namespace z2 {

class Entity;

enum class EntityEventType{
    ///class EntityEvent
    EntityCreated,
    ///class EntityEvent
    EntityRemoved,
    ///class EEntityMoved
    EntityMoved,

    ///class EEntityDamaging
    EntityDamaging,

    ///class EntityEvent
    EntityPerformed,
};

/**
 * Describes an event about an entity.
 */
class EntityEvent : public InGameEvent {
private:
    shared_ptr<Entity> entity;
    EntityEventType entityEventType;
public:
    explicit EntityEvent(shared_ptr<Entity> entityId, EntityEventType type);

    const shared_ptr<Entity> &getEntity() const;
};

class EEntityMoved : public EntityEvent{
private:
    Point from;
    Point dest;
public:
    EEntityMoved(shared_ptr<Entity> entityId, Point from, Point dest);

    const Point &getFrom() const;

    const Point &getDest() const;

};

class EEntityDamaging : public EntityEvent{
private:
    int damage;
    shared_ptr<Entity> attacker;
public:
    EEntityDamaging(shared_ptr<Entity> entityId, shared_ptr<Entity> attacker, int damage);

    int getDamage() const;

    void setDamage(int damage);

    const shared_ptr<Entity> &getAttacker() const;

};




}


#endif //Z2_ENTITYEVENT_H
