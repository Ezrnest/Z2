/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Building.h"

z2::Building::Building(int objectId) : EntityWithHealth(objectId) {}

void z2::Building::deserializeDataPart(istream &input, z2::Building *en) {
    EntityWithHealth::deserializeDataPart(input, en);
}
