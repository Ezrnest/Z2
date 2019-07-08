/*
 * Created by liyicheng on 2019/7/3.
 */

#include "GameUnit.h"
#include "Entity.h"

z2::GameUnit::GameUnit(int objectId) : EntityWithHealth(objectId) {}


void z2::GameUnit::initialize(const Properties &prop) {
    EntityWithHealth::initialize(prop);

}


