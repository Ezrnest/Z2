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

    int getPlayerId() const;
};

}
#endif //Z2_PLAYERMESSAGE_H
