//
// Created by liyicheng on 2019/7/3.
//
#ifndef Z2_ENTITY_H
#define Z2_ENTITY_H


namespace z2{
class Point;
class World;
}

#include <string>
#include "../util/Properties.h"

using namespace ancono;
namespace z2 {
class World;
/**
 * Represents an entity in the game.
 *
 * A concrete sub-class of entity should always define a static function `getIdentifier()` to return the unique
 * identifier
 *
 * Note: Instance of this class (or its subclass) should be held by pointer, using `shared_ptr`.
 */
class Entity {
protected:
    /**
     * The id of the player who owns the game object.
     */
    int ownerId_ = -1;

    /**
     * The id of the game object, which should be unique in the whole game.
     *
     * This id is also used for serialization.
     */
    const int objectId;

    /**
     * The visibility of this object.
     */
    int visibility = 3;
    int movesPerTurn = 3;
    int remainingMoves = 0;
public:

    explicit Entity(int objectId);

    int getOwnerId() const;

    void setOwnerId(int ownerId);

    int getVisibility() const;

    void setVisibility(int visibility);

    const int getObjectId() const;

    virtual const std::string &identifier() const = 0;

    virtual void initialize(const Properties& prop);

    virtual void performAbility(const Point& pos, World& world);

    virtual ~Entity();

    int getMovesPerTurn() const;

    void setMovesPerTurn(int movesPerTurn);

    int getRemainingMoves() const;

    void setRemainingMoves(int remainingMoves);

    void refreshMoves();

    /**
     * Equivalent to `setRemainingMoves(getRemainingMoves() - decrement)`
     */
    void decreaseMoves(int decrement);
};


}
#endif //Z2_ENTITY_H
