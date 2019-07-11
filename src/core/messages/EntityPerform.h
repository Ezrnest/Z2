/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_ENTITYPERFORM_H
#define Z2_ENTITYPERFORM_H

#include "../Message.h"
#include "../../world/Point.h"
#include "GameMessage.h"

namespace z2 {

class EntityPerform : public GameMessage {
private:
    Point pos;
public:
    explicit EntityPerform(const Point &pos);

    EntityPerform();

    const Point &getPos() const;

    void setPos(const Point &pos);

    const string &getClassName()const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};
}

#endif //Z2_ENTITYPERFORM_H
