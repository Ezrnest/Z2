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
using namespace ancono;
namespace z2 {

class UnitBuy : public GameMessage {
private:
    string identifier;
    int playerId;
    /**
     * The position of the building.
     */
    Point pos;
public:
    UnitBuy(const string &identifier, const Point &pos, int playerId);

    UnitBuy();

    /**
     * Gets the identifier (name of the entity) of the entity to buy.
     */
    const string &getIdentifier() const;

    int getPlayerId() const;

    const Point &getPos() const;

    const string &getClassName() const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};

}

#endif //Z2_UNITBUY_H
