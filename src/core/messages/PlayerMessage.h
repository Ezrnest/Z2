/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_PLAYERMESSAGE_H
#define Z2_PLAYERMESSAGE_H

#include "../Message.h"
#include "ControlMessage.h"

namespace z2 {
class PlayerMessage : public ControlMessage{
private:
    int playerId;
public:
    PlayerMessage(ControlMessageType type, int playerId);

    PlayerMessage();

    int getPlayerId() const;

    const string &getClassName() const override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;

};

}
#endif //Z2_PLAYERMESSAGE_H
