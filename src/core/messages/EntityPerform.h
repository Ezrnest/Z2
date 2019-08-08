/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_ENTITYPERFORM_H
#define Z2_ENTITYPERFORM_H

#include "GameMessage.h"
#include "../../world/Point.h"

namespace z2 {
/**
 * Describes the operation that a player performs one entity.
 * Fields are listed below:
 * <ul>
 * <li><code>pos</code>: The position of the entity.
 * </ul>
 *
 */
class EntityPerform : public GameMessage{
private:
    Point pos;
public:

    explicit EntityPerform(const z2::Point &pos);

    EntityPerform();

    const Point &getPos() const;

    const string &getClassName() const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};
}

#endif //Z2_ENTITYPERFORM_H
