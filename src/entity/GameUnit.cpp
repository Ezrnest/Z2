/*
 * Created by liyicheng on 2019/7/3.
 */

#include "GameUnit.h"
#include "Entity.h"
#include "world/Tile.h"
z2::GameUnit::GameUnit(unsigned int objectId) : EntityWithHealth(objectId) {}


void z2::GameUnit::initialize(const Properties &prop) {
    EntityWithHealth::initialize(prop);

}

void z2::GameUnit::serializeDataPart(ostream &output) {
    EntityWithHealth::serializeDataPart(output);
}

void z2::GameUnit::deserializeDataPart(istream &input, z2::GameUnit *en) {
    EntityWithHealth::deserializeDataPart(input, en);
}

int z2::GameUnit::getTileRMP(const z2::Tile &t) {
    return t.getBaseRMP();
}


