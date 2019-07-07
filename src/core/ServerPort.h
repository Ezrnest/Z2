/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_SERVERPORT_H
#define Z2_SERVERPORT_H

#include "Message.h"
#include <memory>
using namespace std;
namespace z2 {
class ServerPort {
private:


public:

    virtual ~ServerPort();

    virtual void sendMessageToServer(const shared_ptr<Message>& message) = 0;

};

}
#endif //Z2_SERVERPORT_H
