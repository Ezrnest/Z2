/*
 * Created by liyicheng on 2019/7/5.
 */

#include "EntityRepository.h"
#include "entity/ConstructionBase.h"
#include "entity/Farmer.h"
#include "plog/Log.h"

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
    properties.set("name", identifier);
    properties.set("className", entityClassInfo->getClassName());
    initEntityTypeInfo();
}

const shared_ptr<EntityClassInfo> &EntityInfo::getEntityClassInfo() const {
    return entityClassInfo;
}

Properties &EntityInfo::getProperties() {
    return properties;
}

void EntityInfo::initEntityTypeInfo() {
    buyable = properties.getBool("buyable", true);
    price = properties.getInt("price", 0);
    requiredTech = properties.getStrSet("requiredTech");
}

int EntityInfo::getPrice() const {
    return price;
}

bool EntityInfo::isBuyable() const {
    return buyable;
}


bool EntityInfo::isBuyableByPlayer(const Player &p) const {
    if(!isBuyable()){
        return false;
    }
    auto& tech = p.getTechnologies();
    for(auto& t : requiredTech){
        if (tech.find(t) == tech.end()) { // no contains
            return false;
        }
    }
    return true;
}

const set<string> &EntityInfo::getRequiredTech() const {
    return requiredTech;
}


EntityRepository &EntityRepository::instance() {
    static EntityRepository repository;
    return repository;
}


EntityInfo &EntityRepository::getEntityInfo(const string &identifier) {
    return entities.at(identifier);
}

vector<string> EntityRepository::getAllLoadedEntityNames() const {
    vector<string> result;
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
    PLOG(plog::info) << ("[EntityRegistry]: Registered class " + className);
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
    PLOG(plog::info) << ("[EntityRegistry]: Registered entity " + info.getIdentifier());
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
    PLOG(plog::info) << "[EntityRegistry]: Init entity classes: Done";
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
    PLOG(plog::info) << ("[EntityRegistry]: Initialized default entities.");
}

bool EntityRepository::hasEntity(const string &entityName) const {
    return entities.find(entityName) != entities.end();
}


void EntityRepository::initFromFolder(const ancono::File &dir) {
    if(!dir.exists()){
        LOG_WARNING << "Unable to load entities from: " << dir.getPath();
        return;
    }

    auto& repo = instance();
    auto entityFiles = dir.listSubfiles();
    for(File& f : entityFiles){
        string eName;
        Properties p;
        ifstream in;
        f.inStream(in);
        p.loadFrom(in);
        eName = p.get("name", f.getFileNameWithoutExtension());
        repo.registerFromProp(eName, p);
        in.close();
    }
//    dir.
}

void EntityRepository::registerFromProp(const string &name, const Properties &properties) {
    auto className = properties.get("className", "");
    auto it = entityClasses.find(className);
    if (it == entityClasses.end()) {
        LOG_WARNING << "Unable to find entity class: [" << className << ']';
        return;
    }
    EntityInfo info(name, (*it).second, properties);
    registerEntity(info);
}

const map<string,EntityInfo> &EntityRepository::getEntityMap() const {
    return entities;
}



