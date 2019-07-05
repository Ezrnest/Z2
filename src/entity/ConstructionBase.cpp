/*
 * Created by liyicheng on 2019/7/4.
 */

#include <string>
#include "ConstructionBase.h"

using namespace std;

z2::ConstructionBase::ConstructionBase(int objectId) : Building(objectId) {}

const std::string &z2::ConstructionBase::identifier() const {
    return ConstructionBase::getIdentifier();
}

std::string &z2::ConstructionBase::getIdentifier() {
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


