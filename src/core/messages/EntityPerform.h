/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_ENTITYPERFORM_H
#define Z2_ENTITYPERFORM_H

#include "../Message.h"
#include "../../world/Point.h"

namespace z2 {

class EntityPerform : public GameMessage {
private:
    Point pos;
public:
    EntityPerform(GeneralMessageType type, const Point &pos);

    const Point &getPos() const;

    void setPos(const Point &pos);
};
}

#endif //Z2_ENTITYPERFORM_H
