/*
 * Created by liyicheng on 2019/7/5.
 */

#include "EntityRepository.h"
#include "../entity/ConstructionBase.h"
#include "../entity/Farmer.h"

using namespace z2;


EntityClassInfo::EntityClassInfo(const string &identifier, const CreatingFunction &creatingFunction) : className(
        identifier),
                                                                                                       creatingFunction(
                                                                                                               creatingFunction) {}

const string &EntityClassInfo::getClassName() const {
    return className;
}

void EntityClassInfo::setClassName(const string &identifier) {
    EntityClassInfo::className = identifier;
}

const Properties &EntityClassInfo::getProperties() const {
    return properties;
}

void EntityClassInfo::setProperties(const Properties &properties) {
    EntityClassInfo::properties = properties;
}

const CreatingFunction &EntityClassInfo::getCreatingFunction() const {
    return creatingFunction;
}

Properties &EntityClassInfo::getProperties() {
    return properties;
}



const string &EntityInfo::getIdentifier() const {
    return identifier;
}

const Properties &EntityInfo::getProperties() const {
    return properties;
}

EntityInfo::EntityInfo(const string &identifier, const shared_ptr<EntityClassInfo> &entityClassInfo,
                       const Properties &properties) : identifier(identifier), entityClassInfo(entityClassInfo),
                                                       properties(properties) {}

const shared_ptr<EntityClassInfo> &EntityInfo::getEntityClassInfo() const {
    return entityClassInfo;
}

Properties &EntityInfo::getProperties() {
    return properties;
}


EntityRepository &EntityRepository::instance() {
    static EntityRepository repository;
    return repository;
}


EntityInfo &EntityRepository::getEntityInfo(const string &identifier) {
    return entities.at(identifier);
}

const vector<string> EntityRepository::getAllLoadedEntityNames() const {
    vector<string> result(entities.size());
    for (auto &en : entities) {
        result.push_back(en.first);
    }
    return result;
}

shared_ptr<Entity> EntityRepository::createEntity(const string &entityName, int objectId) {
    const EntityInfo &info = getEntityInfo(entityName);
    const auto &f = info.getEntityClassInfo()->getCreatingFunction();
    Entity* entity = f(objectId, info.getProperties());
    return shared_ptr<Entity>(entity);
}


shared_ptr<Entity>
EntityRepository::createEntity(const string &entityName, int objectId, const Properties &initialProperties) {
    auto entity = createEntity(entityName,objectId);
    entity->initialize(initialProperties);
    return entity;
}

void EntityRepository::registerEntityClass(const string &className, const CreatingFunction &creatingFunction) {
    shared_ptr<EntityClassInfo> info(new EntityClassInfo(className, creatingFunction));
    entityClasses.insert(make_pair(className, info));
}


const map<string, shared_ptr<EntityClassInfo>> &EntityRepository::getAllLoadedEntityClasses() const {
    return entityClasses;
}
EntityRepository::EntityRepository() = default;
void EntityRepository::registerEntity(const EntityInfo &info) {
    entities.insert(make_pair(info.getIdentifier(), info));
}

void EntityRepository::initEntityClasses() {
    EntityRepository &repo = EntityRepository::instance();

    //Buildings:
    //ConstructionBase:
    repo.registerEntityClass(ConstructionBase::getIdentifier(), ConstructionBase::create);

    repo.registerEntityClass(Farmer::getIdentifier(), Farmer::create);

}

void EntityRepository::initDefaultEntities() {
    initEntityClasses();
    auto& repo = instance();
    for(auto& en : repo.entityClasses){
        auto cInfo = en.second;
        EntityInfo info(cInfo->getClassName(),cInfo,Properties());
        repo.registerEntity(info);
    }
}


