/*
 * Created by liyicheng on 2019/7/9.
 */

#include <core/messages/SignalMessage.h>
#include <core/messages/PlayerMessage.h>
#include "SerializableRegistry.h"
#include "world/World.h"
#include "entity/Farmer.h"
#include "entity/ConstructionBase.h"
#include "core/messages/ControlMessage.h"
#include "core/messages/UnitAttack.h"
#include "core/messages/UnitBuy.h"
#include "core/messages/UnitMove.h"
#include "core/messages/EntityPerform.h"
#include "core/messages/RegisterPlayer.h"
#include "core/messages/EntityKill.h"
#include "core/messages/SyncWorld.h"
#include "plog/Log.h"
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
    
    auto it = nameMap.find(className);
    if(it == nameMap.end()){
        PLOG_WARNING << "[SerializableRegistry] Unregistered class name: [" << className << ']';
        return nullptr;
    }
    PLOG_INFO << "[SerializableRegistry] Deserialize:[" << className << ']';
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
    registerMessageClass<PlayerMessage>(sr);
    registerMessageClass<RegisterPlayer>(sr);
    registerMessageClass<SyncWorld>(sr);
    registerMessageClass<SignalMessage>(sr);


    registerMessageClass<EntityPerform>(sr);
    registerMessageClass<EntityKill>(sr);

    registerMessageClass<UnitAttack>(sr);
    registerMessageClass<UnitBuy>(sr);
    registerMessageClass<UnitMove>(sr);
    //TODO add message classes
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
    static_assert(is_base_of<DataSerializable, Clazz>::value, "The class should be a subclass of Message!");
    auto *msg = new Clazz();
    msg->deserializeData(input);
    return msg;
}
