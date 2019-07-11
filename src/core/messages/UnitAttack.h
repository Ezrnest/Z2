/*
 * Created by liyicheng on 2019/7/11.
 */

#ifndef Z2_UNITATTACK_H
#define Z2_UNITATTACK_H

#include "GameMessage.h"
#include "../../world/Point.h"

namespace z2 {
class UnitAttack : public GameMessage {
private:
    Point from;
    Point dest;
public:
    UnitAttack(const Point &from, const Point &dest);

    UnitAttack();

    const Point &getFrom() const;

    const Point &getDest() const;

    const string &getClassName()const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;


};
}

#endif //Z2_UNITATTACK_H
