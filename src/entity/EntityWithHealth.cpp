/*
 * Created by liyicheng on 2019/7/3.
 */

#include "EntityWithHealth.h"

z2::EntityWithHealth::EntityWithHealth(int objectId)
        : Entity(objectId),
          health(100),
          maxHealth(100) {}

z2::EntityWithHealth::~EntityWithHealth() = default;

int z2::EntityWithHealth::getHealth() const {
    return health;
}

void z2::EntityWithHealth::setHealth(int health) {
    EntityWithHealth::health = health;
}

int z2::EntityWithHealth::getMaxHealth() const {
    return maxHealth;
}

void z2::EntityWithHealth::setMaxHealth(int maxHealth) {
    EntityWithHealth::maxHealth = maxHealth;
}

void z2::EntityWithHealth::initialize(const Properties &prop) {
    Entity::initialize(prop);
    //TODO
    //health = prop.getInt("health", health);
    //maxHealth = prop.getInt("maxHealth", maxHealth);
}
