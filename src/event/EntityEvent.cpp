#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by liyicheng on 2019/7/26.
//

#include "EntityEvent.h"

namespace z2 {


EntityEvent::EntityEvent(shared_ptr<Entity> entity,EntityEventType type) : entity(std::move(entity)), entityEventType(type) {}

const shared_ptr<Entity> &EntityEvent::getEntity() const {
    return entity;
}

EEntityMoved::EEntityMoved(shared_ptr<Entity> entityId, Point from, Point dest)
        : EntityEvent(std::move(entityId), EntityEventType::EntityMoved), from(std::move(from)), dest(std::move(dest)) {}

const Point &EEntityMoved::getFrom() const {
    return from;
}

const Point &EEntityMoved::getDest() const {
    return dest;
}


EEntityDamaging::EEntityDamaging(shared_ptr<Entity> entityId, shared_ptr<Entity> attacker, int damage)
        : EntityEvent(std::move(entityId), EntityEventType::EntityDamaging),
          damage(damage), attacker(attacker) {}

int EEntityDamaging::getDamage() const {
    return damage;
}

void EEntityDamaging::setDamage(int damage) {
    EEntityDamaging::damage = damage;
}

const shared_ptr<Entity> &EEntityDamaging::getAttacker() const {
    return attacker;
}

}