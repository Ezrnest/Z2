/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Building.h"

z2::Building::Building(int objectId) : EntityWithHealth(objectId) {}



void z2::Building::serializeTo(ostream &output) {
    output << className() << ' ';
    serializeDataPart(output);
}

const string &z2::Building::getClassName() const {
    return className();
}

void z2::Building::initialize(const Properties &prop) {
    EntityWithHealth::initialize(prop);

}

void z2::Building::serializeDataPart(ostream &output) {
    EntityWithHealth::serializeDataPart(output);

}

void z2::Building::deserializeDataPart(istream &input, z2::Building *en) {
    EntityWithHealth::deserializeDataPart(input, en);

}

z2::Building *z2::Building::create(int objectId, const Properties &initializer) {
    auto* b = new Building(objectId);
    b->initialize(initializer);
    return b;
}

std::string &z2::Building::className() {
    static string name = "Building";
    return name;
}

z2::Building *z2::Building::loadFrom(istream &input) {
    auto* b = new Building(0);
    Building::deserializeDataPart(input, b);
    return b;
}
