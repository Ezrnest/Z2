/*
 * Created by liyicheng on 2019/7/9.
 */

#include "SerializableRegistry.h"
#include "../world/World.h"
#include "../entity/Farmer.h"
#include "../entity/ConstructionBase.h"
#include "messages/ControlMessage.h"
#include "messages/EntityPerform.h"
#include "messages/UnitAttack.h"
#include "messages/UnitBuy.h"
#include "messages/UnitMove.h"
#include "messages/UnitPerform.h"

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

namespace z2 {



template<typename MClass>
void registerMessageClass(SerializableRegistry &sr) {
    static_assert(is_base_of<Message, MClass>::value, "The class must be a subclass of Message!");
    MClass ins;
    sr.registerClass(ins.getClassName(), SerializableRegistry::deserializeT<MClass>);
}

void initMessageClass(SerializableRegistry &sr) {
    registerMessageClass<ControlMessage>(sr);
    registerMessageClass<EntityPerform>(sr);
    registerMessageClass<UnitAttack>(sr);
    registerMessageClass<UnitBuy>(sr);
    registerMessageClass<UnitMove>(sr);
    registerMessageClass<UnitPerform>(sr);
}
}

void z2::SerializableRegistry::initSerializableClasses() {
    //TODO add more classes
    auto &sr = instance();
    sr.registerClass<World>();
    sr.registerClass<Farmer>();
    sr.registerClass<ConstructionBase>();
    sr.registerClass<MeleeUnit>();
    sr.registerClass<RangeUnit>();

    z2::initMessageClass(sr);
}


template<typename Clazz>
void SerializableRegistry::registerClass() {
    registerClass(Clazz::className(), Clazz::loadFrom);
}

template<typename Clazz>
Clazz *SerializableRegistry::deserializeT(istream &input) {
    static_assert(is_base_of<Message, Clazz>::value, "The base class should be a ");
    auto *msg = new Clazz();
    msg->deserializeData(input);
    return msg;
}
