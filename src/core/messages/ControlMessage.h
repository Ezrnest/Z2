/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_CONTROLMESSAGE_H
#define Z2_CONTROLMESSAGE_H


#include "../Message.h"

namespace z2 {
enum class ControlMessageType {
    StartGame, EndGame,
    PlayerTurnStart, PlayerTurnFinish,
    PlayerDefeated, PlayerWin,

};

/**
 * Describes the control message of the game, such as start of the game.
 */
class ControlMessage : public Message {
private:
    ControlMessageType controlType;
public:
    ControlMessage(ControlMessageType type1);

    ControlMessageType getControlType() const;
};
}


#endif //Z2_CONTROLMESSAGE_H
