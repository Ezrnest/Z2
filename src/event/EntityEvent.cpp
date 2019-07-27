#include <utility>

#include <utility>

//
// Created by liyicheng on 2019/7/26.
//

#include "EntityEvent.h"

namespace z2 {


EntityEvent::EntityEvent(int entityId,EntityEventType type) : entityId(entityId), entityEventType(type) {}

int EntityEvent::getEntityId() const {
    return entityId;
}

EEntityMoved::EEntityMoved(int entityId, Point from, Point dest)
        : EntityEvent(entityId, EntityEventType::EntityMoved), from(std::move(from)), dest(std::move(dest)) {}

const Point &EEntityMoved::getFrom() const {
    return from;
}

const Point &EEntityMoved::getDest() const {
    return dest;
}

EEntityDamaged::EEntityDamaged(int entityId, int damage) : EntityEvent(entityId, EntityEventType::EntityDamaged),
                                                           damage(damage) {}

int EEntityDamaged::getDamage() const {
    return damage;
}

void EEntityDamaged::setDamage(int damage) {
    EEntityDamaged::damage = damage;
}
}