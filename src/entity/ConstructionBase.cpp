/*
 * Created by liyicheng on 2019/7/4.
 */

#include <string>
#include "ConstructionBase.h"
#include "world/World.h"

using namespace std;

z2::ConstructionBase::ConstructionBase(unsigned int objectId) : Building(objectId) {}

const std::string &z2::ConstructionBase::getClassName() const {
    return ConstructionBase::className();
}

std::string &z2::ConstructionBase::className() {
    static string name = "ConstructionBase";
    return name;
}


z2::ConstructionBase *z2::ConstructionBase::create(int objectId, const Properties &initializer) {
    auto entity = new ConstructionBase(objectId);
    entity->initialize(initializer);
    return entity;
}

void z2::ConstructionBase::initialize(const Properties &prop) {
    Building::initialize(prop);
}

void z2::ConstructionBase::deserializeDataPart(istream &input, z2::ConstructionBase *en) {
    Building::deserializeDataPart(input, en);
    input >> en->productionDirection;
}

void z2::ConstructionBase::buyEntity(const z2::Point &pos, const string &name, z2::World &world) {
    auto &directions = Point::directions();
    Point place;
    for (int i = 0; i < directions.size(); i++) {
        const Point &d = directions[(productionDirection + i) % directions.size()];
        place = pos + d;
        if (world.isInside(place) && !world.getTile(place).isOccupied()) {
            break;
        }
    }
    world.createEntity(place, name, getOwnerId());
}


z2::ConstructionBase *z2::ConstructionBase::loadFrom(istream &input) {
    auto *base = new ConstructionBase(0);
    ConstructionBase::deserializeDataPart(input, base);
    return base;
}

void z2::ConstructionBase::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
}

void z2::ConstructionBase::serializeDataPart(ostream &output) {
    z2::Building::serializeDataPart(output);
    output << productionDirection << ' ';
}

void z2::ConstructionBase::performAbility(const z2::Point &pos, z2::World &world) {
    productionDirection++;
    this->setRemainingMoves(1);// trick to make it always performable
}

bool z2::ConstructionBase::canPerformAbility(const z2::Point &pos, z2::World &world) {
    return true;
}

int z2::ConstructionBase::getProductionDirection() const {
    return productionDirection;
}


