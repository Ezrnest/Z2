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
    explicit ControlMessage(ControlMessageType type1);

    ControlMessage();

    ControlMessageType getControlType() const;

    const string &getClassName()const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;
};
}


#endif //Z2_CONTROLMESSAGE_H
