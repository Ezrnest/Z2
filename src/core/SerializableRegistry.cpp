/*
 * Created by liyicheng on 2019/7/9.
 */

#include "SerializableRegistry.h"
#include "../world/World.h"
#include "../entity/Farmer.h"
#include "../entity/ConstructionBase.h"

using namespace z2;
void z2::SerializableRegistry::registerClass(const string &className, const z2::DeserializingFunction &df) {
    nameMap.insert(make_pair(className, df));
}

const z2::DeserializingFunction &z2::SerializableRegistry::getDFunction(const string &className) {
    return nameMap[className];
}

z2::SerializableRegistry &z2::SerializableRegistry::instance() {
    static SerializableRegistry registry;
    return registry;
}


z2::Serializable *z2::SerializableRegistry::deserialize(istream &input) {
    string className;
    input >> className;
    auto &df = nameMap[className];
    return df(input);
}

void z2::SerializableRegistry::initSerializableClasses() {
    //TODO add more classes
    auto& sr = instance();
    sr.registerClass(World::className(),World::loadFrom);
    sr.registerClass(Farmer::className(), Farmer::loadFrom);
    sr.registerClass(ConstructionBase::className(), ConstructionBase::loadFrom);

}
