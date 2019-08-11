//
// Created by liyicheng on 2019/7/3.
//

#include <config/EntityRepository.h>
#include "GameUnit.h"
#include "Entity.h"

using namespace z2;

Entity::Entity(const unsigned int objectId) : objectId(objectId) {}

Entity::~Entity() = default;

int Entity::getOwnerId() const {
    return ownerId_;
}

void Entity::setOwnerId(int ownerId) {
    Entity::ownerId_ = ownerId;
}

int Entity::getObjectId() const {
    return objectId;
}

void Entity::initialize(const Properties &prop) {
    //TODO
    entityName = prop.get("entityName", entityName);
    ownerId_ = prop.getInt("ownerId", ownerId_);
    movesPerTurn = prop.getInt("movesPerTurn", movesPerTurn);
    visibility = prop.getInt("visibility", visibility);
}

void Entity::performAbility(const Point &pos, World &world) {
    //empty
}

int Entity::getVisibility() const {
    return visibility;
}

void Entity::setVisibility(int visibility) {
    Entity::visibility = visibility;
}


int Entity::getMovesPerTurn() const {
    return movesPerTurn;
}

void Entity::setMovesPerTurn(int movesPerTurn) {
    Entity::movesPerTurn = movesPerTurn;
}

int Entity::getRemainingMoves() const {
    return remainingMoves;
}

void Entity::setRemainingMoves(int remainingMoves) {
    Entity::remainingMoves = remainingMoves;
}

void Entity::refreshMoves() {
    remainingMoves = movesPerTurn;
}

bool Entity::requireMoves(int count) {
    if (remainingMoves >= count) {
        remainingMoves -= count;
        return true;
    } else {
        return false;
    }
}

void Entity::serializeDataPart(ostream &output) {
    output << entityName << ' '
           << objectId << ' '
           << ownerId_ << ' ';
    pos.serializeData(output);
    output << visibility << ' '
           << movesPerTurn << ' '
           << remainingMoves << ' ';

}

void Entity::deserializeDataPart(istream &input, Entity *en) {
    input >> en->entityName
          >> en->objectId
          >> en->ownerId_;
    en->pos.deserializeData(input);
    input >> en->visibility
          >> en->movesPerTurn
          >> en->remainingMoves;
}

const string &Entity::getEntityName() const {
    return entityName;
}

bool Entity::requireRestMoves() {
    if (remainingMoves > 0) {
        remainingMoves = 0;
        return true;
    } else {
        return false;
    }
}

const Point &Entity::getPos() const {
    return pos;
}

void Entity::setPos(const Point &pos) {
    Entity::pos = pos;
}

bool Entity::canPerformAbility(const Point &pos, World &world) {
    return false;
}

const EntityInfo &Entity::getEntityInfo() {
    auto& repo = EntityRepository::instance();
    return repo.getEntityInfo(entityName);
}
