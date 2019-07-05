/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_UNITBUY_H
#define Z2_UNITBUY_H

#include "../Message.h"
#include "../../world/Point.h"
#include <string>
using namespace std;
namespace z2 {

class UnitBuy : public GameMessage {
private:
    const string identifier;
    const int playerId;
    const Point pos;
public:
    UnitBuy(const string &identifier, const z2::Point &pos, int playerId);

    const string &getIdentifier() const;

    int getPlayerId() const;

    const Point &getPos() const;
};

}

#endif //Z2_UNITBUY_H
