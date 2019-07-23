//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_ENTITYKILL_H
#define Z2_ENTITYKILL_H

#include "GameMessage.h"
#include "../../world/Point.h"

namespace z2 {

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
