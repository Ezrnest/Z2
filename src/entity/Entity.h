//
// Created by liyicheng on 2019/7/3.
//
#ifndef Z2_ENTITY_H
#define Z2_ENTITY_H


namespace z2 {
class Point;

class World;
class Tile;
class EntityInfo;
}

#include <string>
#include "../core/Serializable.h"
#include "../util/Properties.h"
#include "world/Point.h"

using namespace ancono;
namespace z2 {

/**
 * Represents a type of entity in the game.
 * <h2>Entity Classes and Entities</h2>
 * The c++ class `Entity` represents an entity class, but there is no one-to-one mapping from
 * the entity classes to the types of entities in the game. Different types of entity in the game
 * can be represented by a single entity class. For convenience, the word `entity` refers to a
 * type of entity.
 *
 * <h2>Entity Registration</h2>
 * In the game, entities are managed through the entity registration. Generally, an entity registration
 * file is loaded to register all the entities the the game. After registration, entities will be
 * available in the entity repository.
 *
 * <h2>Creating Entities</h2>
 * In the game, entities are created through the entity repository. With the name(identifier) of a type
 * of entity, a creating function will be
 * invoked to create a new instance of an entity and initialization will be done, using the properties
 * of the corresponding entity type.
 *
 * <h2>Implementation Notes</h2>
 * A concrete sub-class of entity should always define a static function <code>getClassName()</code>
 * to return the unique identifier.
 *
 *
 * <br>Note: Instance of this class (or its subclass) should be held by pointer, using `shared_ptr`.
 */
class Entity : public Serializable {
protected:
    /**
     * The entity typename.
     */
    string entityName;
    /**
     * The id of the game object, which should be unique in the whole game. 0 for an invalid id.
     *
     * This id is also used for serialization.
     */
    unsigned int objectId;

    /**
     * The current position of this entity.
     */
    Point pos;

    /**
     * The id of the player who owns the game object.
     */
    int ownerId_ = -1;

    /**
     * The visibility of this object.
     */
    int visibility = 3;
    int movesPerTurn = 3;

    /**
     * Transient.
     */
    int remainingMoves = 0;

    /**
     * Serialize only the entity part to the output.
     */
    virtual void serializeDataPart(ostream &output);
    /**
     * Deserialize only the entity part to the output.
     */
    static void deserializeDataPart(istream &input, Entity *en);

public:

    explicit Entity(unsigned int objectId);

    const string &getEntityName() const;

    int getOwnerId() const;

    void setOwnerId(int ownerId);

    int getVisibility() const;

    void setVisibility(int visibility);

    int getObjectId() const;

    /**
     * Gets the class name of this entity.
     */
    const std::string &getClassName() const override = 0;

    virtual void initialize(const Properties &prop);

    virtual void performAbility(const Point &pos, World &world);

    virtual bool canPerformAbility(const Point &pos, World &world);

    virtual ~Entity();

    int getMovesPerTurn() const;

    void setMovesPerTurn(int movesPerTurn);

    int getRemainingMoves() const;

    void setRemainingMoves(int remainingMoves);

    const Point &getPos() const;

    void setPos(const Point &pos);

    void refreshMoves();

    /**
     * Requires an entity the given count of moves. If it has
     * sufficient moves, its `remainingMoves` will be decreased by the `count`, and `true` will be returned.
     * Otherwise, its `remainingMoves` remains the same and `false` will be returned.
     */
    bool requireMoves(int count);

    /**
     * If this entity has at least one move, this method will return `true`, otherwise `false`.
     * After this method, the `remainingMoves` of this entity will be set to zero.
     */
    bool requireRestMoves();



    const EntityInfo& getEntityInfo();
};


}
#endif //Z2_ENTITY_H
