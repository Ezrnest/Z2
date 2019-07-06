/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_UNITBUY_H
#define Z2_UNITBUY_H

#include "../Message.h"
#include "../../world/Point.h"
#include "GameMessage.h"
#include "../../util/Properties.h"
#include <string>
using namespace std;
namespace z2 {

class UnitBuy : public GameMessage {
private:
    const string identifier;
    const int playerId;
    /**
     * The position of the building.
     */
    const Point pos;
    Properties prop;
public:
    UnitBuy(const string &identifier, const Point &pos, int playerId);

    UnitBuy(const string &identifier, int playerId, const Point &pos,
            const Properties &prop);

    const string &getIdentifier() const;

    int getPlayerId() const;

    const Point &getPos() const;

    const Properties &getProp() const;

    void setProp(const Properties &prop);
};

}

#endif //Z2_UNITBUY_H
