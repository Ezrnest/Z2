//
// Created by liyicheng on 2019/7/3.
//
#ifndef Z2_ENTITY_H
#define Z2_ENTITY_H

#include <string>
#include "../util/Properties.h"
#include "../world/World.h"


namespace z2 {

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

public:

    explicit Entity(int objectId);

    int getOwnerId() const;

    void setOwnerId(int ownerId);

    const int getObjectId() const;

    virtual const std::string &identifier() const = 0;

    virtual void initialize(const Properties& prop);

    virtual void performAbility(const Point& pos, World& world);

    virtual ~Entity();
};


}
#endif //Z2_ENTITY_H
