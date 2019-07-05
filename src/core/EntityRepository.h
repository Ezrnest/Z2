/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_ENTITYREPOSITORY_H
#define Z2_ENTITYREPOSITORY_H

#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include "../util/Properties.h"
#include "../entity/Entity.h"

using namespace std;

namespace z2 {
using CreatingFunction = function<Entity *(int, const Properties &)>;

class EntityClassInfo {
private:
    string className;
    Properties properties;
    CreatingFunction creatingFunction;
public:
    EntityClassInfo(const string &identifier, const CreatingFunction &creatingFunction);

    const string &getClassName() const;

    void setClassName(const string &identifier);

    const Properties &getProperties() const;

    Properties &getProperties();

    void setProperties(const Properties &properties);

    const CreatingFunction &getCreatingFunction() const;
};

class EntityInfo {
private:
    const string identifier;
    const shared_ptr<EntityClassInfo> entityClassInfo;
    Properties properties;
public:

    EntityInfo(const string &identifier, const shared_ptr<EntityClassInfo> &entityClassInfo,
               const Properties &properties);


    const shared_ptr<EntityClassInfo> &getEntityClassInfo() const;

    const string &getIdentifier() const;

    const Properties &getProperties() const;

    Properties &getProperties();

};

/**
 * In charge of the registering of entities in the game. An entity in the game is associated
 * with a entity class, which is represented by `EntityClassInfo`, and some `Properties`.
 */
class EntityRepository {


    map<string, shared_ptr<EntityClassInfo>> entityClasses;

    map<string, EntityInfo> entities;

    EntityRepository();

public:
    EntityRepository(const EntityRepository &entityRepository) = delete;

    EntityRepository(EntityRepository &&entityRepository) noexcept = delete;

    EntityRepository &operator=(const EntityRepository &entityRepository) = delete;

    EntityRepository &operator=(EntityRepository &&entityRepository) noexcept = delete;

    /**
     * Register an entity to the repository.
     */
    void registerEntity(const EntityInfo &info);

    /**
     * Register an entity class to the repository.
     */
    void registerEntityClass(const string &className, const CreatingFunction &creatingFunction);

    EntityInfo &getEntityInfo(const string &identifier);


    const vector<string> getAllLoadedEntityNames() const;

    const map<string, shared_ptr<EntityClassInfo>> &getAllLoadedEntityClasses() const;

    /**
     * Creates an entity with the given entity name.
     */
    shared_ptr<Entity> createEntity(const string &entityName, int objectId, const Properties &initialProperties);

    shared_ptr<Entity> createEntity(const string &entityName, int objectId);

    /**
     * Gets the instance of entity repository.
     */
    static EntityRepository &instance();

    /**
     * Loads the entity classes.
     */
    static void initEntityClasses();
};

}


#endif //Z2_ENTITYREPOSITORY_H
