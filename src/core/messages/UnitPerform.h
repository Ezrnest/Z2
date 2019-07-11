/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_UNITPERFORM_H
#define Z2_UNITPERFORM_H

#include "GameMessage.h"
#include "../../world/Point.h"

namespace z2 {
class UnitPerform : public GameMessage{
private:
    Point pos;
public:

    explicit UnitPerform(const z2::Point &pos);

    UnitPerform();

    const Point &getPos() const;

    const string &getClassName() const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};
}

#endif //Z2_UNITPERFORM_H
