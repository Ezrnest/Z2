/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_GAMEMESSAGE_H
#define Z2_GAMEMESSAGE_H


#include "../Message.h"

namespace z2{

/**
 * This enum class contains all the types of game message. Generally, the class named the same as the
 * name of the enum constant is the corresponding sub-class of `GameMessage`. See the documentation of
 * those class for more info.
 */
enum class GameMessageType{

    //Game Message:
    ///class: UnitBuy
    UnitBuy,
    ///class: TechResearch
    TechResearch,

    //Entity Message:
    ///class: EntityPerform
    EntityPerform,
    ///class: EntityKill
    EntityKill,
    ///class: UnitAttack
    UnitAttack,
    ///class: UnitMove
    UnitMove,

    ///class: SetPlayerData <br> This message is used for bot to cheat.
    SetPlayerData,
};

/**
 * <h2>Overview</h2>
 * Class GameMessage describes the message about the game itself. All game messages are created by the player and
 * it represents an operation of the player.
 * The client sends the message to the server and then the server will broadcast the message to all the clients.
 * A client will process the message when it receives the message.
 * <h2>Game Message Types</h2>
 * The enum field `gameType` describes the type of the game message. See enum class `GameMessageType` for all the
 * types of game message.
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
