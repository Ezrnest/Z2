/*
 * Created by liyicheng on 2019/7/4.
 */

#include <string>
#include "ConstructionBase.h"

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
    EntityWithHealth::initialize(prop);
}

void z2::ConstructionBase::deserializeDataPart(istream &input, z2::ConstructionBase *en) {
    Building::deserializeDataPart(input, en);
}

z2::ConstructionBase *z2::ConstructionBase::loadFrom(istream &input) {
    auto* base = new ConstructionBase(0);
    ConstructionBase::deserializeDataPart(input, base);
    return base;
}

void z2::ConstructionBase::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
}

void z2::ConstructionBase::serializeDataPart(ostream &output) {
    EntityWithHealth::serializeDataPart(output);
}


