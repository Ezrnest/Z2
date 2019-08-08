//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_ENTITYKILL_H
#define Z2_ENTITYKILL_H

#include "GameMessage.h"
#include "../../world/Point.h"

namespace z2 {
/**
 * Describes the operation that the player kills one unit, which usually belongs to the player.
 * Fields are listed below:
 * <ul>
 * <li><code>pos</code>: The position of the entity.
 * </ul>
 *
 */
class EntityKill : public GameMessage{
private:
    Point pos;
public:
    explicit EntityKill(z2::Point pos);

    EntityKill();

    const Point &getPos() const;

    const string &getClassName() const override;

    void deserializeData(istream &input) override;

protected:
    void serializeData(ostream &output) override;
};
}

#endif //Z2_ENTITYKILL_H
