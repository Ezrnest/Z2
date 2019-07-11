/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_GAMEMESSAGE_H
#define Z2_GAMEMESSAGE_H


#include "../Message.h"

namespace z2{

enum class GameMessageType{

    //Game Message:
    UnitBuy,

    //Entity Message:
    UnitMove, UnitPerform, UnitAttack,
};

/**
 * Describes the message about the game itself.
 */
class GameMessage : public Message {
private:
    GameMessageType gameType;
public:
    explicit GameMessage(GameMessageType type);

    GameMessageType getGameType() const;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};

}

#endif //Z2_GAMEMESSAGE_H
