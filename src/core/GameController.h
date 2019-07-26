/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_GAMECONTROLLER_H
#define Z2_GAMECONTROLLER_H
namespace z2 { class GameGui; }

#include "Message.h"

using namespace std;
namespace z2 {
/**
 * Describes the controller at the client side.
 */
class GameController {
public:
    /**
    * Sends the given message to the server.
    *
    * This method should be invoked logically by the player(/view) part.
    */
    virtual void sendMessageToServer(const MessagePtr &message) = 0;

    virtual void setView(const shared_ptr<GameGui> &gui) = 0;
    
    virtual bool isGameRunning() = 0;
};
}

#endif //Z2_GAMECONTROLLER_H
