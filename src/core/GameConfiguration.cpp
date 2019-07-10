/*
 * Created by liyicheng on 2019/7/10.
 */

#include "GameConfiguration.h"
#include "EntityRepository.h"
#include "SerializableRegistry.h"

using namespace z2;
void GameConfiguration::initRegistration() {
    EntityRepository::initDefaultEntities();
    //TODO load from file

    SerializableRegistry::initSerializableClasses();
}
