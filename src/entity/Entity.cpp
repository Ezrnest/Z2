//
// Created by liyicheng on 2019/7/3.
//

#include "Entity.h"
using namespace z2;

Entity::Entity(const int objectId) : objectId(objectId) {}
Entity::~Entity() = default;
int Entity::getOwnerId() const {
    return ownerId_;
}

void Entity::setOwnerId(int ownerId) {
    Entity::ownerId_ = ownerId;
}

const int Entity::getObjectId() const {
    return objectId;
}

void Entity::initialize(const Properties &prop) {
    //TODO
    //ownerId_ = prop.getInt("ownerId");
}

void Entity::performAbility(const Point &pos, World &world) {
    //empty
}


