/*
 * Created by liyicheng on 2019/7/3.
 */

#include "EntityWithHealth.h"

using namespace z2;

EntityWithHealth::EntityWithHealth(unsigned int objectId)
        : Entity(objectId),
          health(100),
          maxHealth(100) {}

EntityWithHealth::~EntityWithHealth() = default;

int EntityWithHealth::getHealth() const {
    return health;
}

void EntityWithHealth::setHealth(int health) {
    EntityWithHealth::health = health;
}

int EntityWithHealth::getMaxHealth() const {
    return maxHealth;
}

void EntityWithHealth::setMaxHealth(int maxHealth) {
    EntityWithHealth::maxHealth = maxHealth;
}

void EntityWithHealth::initialize(const Properties &prop) {
    Entity::initialize(prop);
    //TODO
    maxHealth = prop.getInt("maxHealth", maxHealth);
    health = prop.getInt("health", maxHealth); // set the current health to max
}

void EntityWithHealth::serializeDataPart(ostream &output) {
    Entity::serializeDataPart(output);
    output << health << ' '
           << maxHealth << ' ';
}

void EntityWithHealth::deserializeDataPart(istream &input, EntityWithHealth *en) {
    Entity::deserializeDataPart(input, en);
    input >> en->health
          >> en->maxHealth;
}

bool EntityWithHealth::isDead() {
    return health <= 0;
}

bool EntityWithHealth::dealDamage(int damage) {
    health -= damage;
    return isDead();
}
