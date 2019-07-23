/*
 * Created by liyicheng on 2019/7/5.
 */

#include "EntityRepository.h"
#include "../entity/ConstructionBase.h"
#include "../entity/Farmer.h"
#include "../util/LogUtil.h"
#include "../lib/easylogging++.h"

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


/**
 * Gets the identifier(name) of this entity.
 */
const string &EntityInfo::getIdentifier() const {
    return identifier;
}

const Properties &EntityInfo::getProperties() const {
    return properties;
}

EntityInfo::EntityInfo(const string &identifier, const shared_ptr<EntityClassInfo> &entityClassInfo,
                       const Properties &prop) : identifier(identifier), entityClassInfo(entityClassInfo),
                                                       properties(prop) {
    properties.set("entityName", identifier);
}

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
    try{
        const EntityInfo &info = getEntityInfo(entityName);
        const auto &f = info.getEntityClassInfo()->getCreatingFunction();
        Entity *entity = f(objectId, info.getProperties());
        return shared_ptr<Entity>(entity);
    }catch (...){
        return shared_ptr<Entity>();
    }

}


shared_ptr<Entity>
EntityRepository::createEntity(const string &entityName, int objectId, const Properties &initialProperties) {
    auto entity = createEntity(entityName, objectId);
    entity->initialize(initialProperties);
    return entity;
}

void EntityRepository::registerEntityClass(const string &className, const CreatingFunction &creatingFunction) {
    shared_ptr<EntityClassInfo> info(new EntityClassInfo(className, creatingFunction));
    entityClasses.insert(make_pair(className, info));
    LOG(INFO) << ("[EntityRegistry]: Registered class " + className);
}


const map<string, shared_ptr<EntityClassInfo>> &EntityRepository::getAllLoadedEntityClasses() const {
    return entityClasses;
}

EntityRepository::EntityRepository() = default;

/**
 * Register an entity to the repository. If there exists an entity of the same identifier, this method simply returns.
 */
void EntityRepository::registerEntity(const EntityInfo &info) {
    entities.insert(make_pair(info.getIdentifier(), info));
    LOG(INFO) << ("[EntityRegistry]: Registered entity " + info.getIdentifier());
}

/**
 * Initializes the entity classes.
 *
 * Note: newly created entity sub-classes should be included in this function
 */
void EntityRepository::initEntityClasses() {

    EntityRepository &repo = EntityRepository::instance();

    //Buildings:
    //ConstructionBase:
    repo.registerEntityClass(ConstructionBase::className(), ConstructionBase::create);

    repo.registerEntityClass(Farmer::className(), Farmer::create);

    //TODO
    repo.registerEntityClass(MeleeUnit::className(), MeleeUnit::create);
    repo.registerEntityClass(RangeUnit::className(), RangeUnit::create);
    LOG(INFO) << "[EntityRegistry]: Init entity classes: Done";
}

/**
 * Initializes the default entities. An entity type will be created for each loaded entity classes.
 * The name of the entity will be the same as the name of the entity class.
 *
 * Note: This method will invoke `initEntityClasses()` firstly.
 */
void EntityRepository::initDefaultEntities() {
    initEntityClasses();
    auto &repo = instance();
    for (auto &en : repo.entityClasses) {
        auto cInfo = en.second;
        EntityInfo info(cInfo->getClassName(), cInfo, Properties());
        repo.registerEntity(info);
    }
    LOG(INFO) << ("[EntityRegistry]: Initialized default entities.");
}

bool EntityRepository::hasEntity(const string &entityName) const {
    return entities.find(entityName) != entities.end();
}


