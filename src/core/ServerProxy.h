/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_SERVERPROXY_H
#define Z2_SERVERPROXY_H

#include "Message.h"
#include <memory>
using namespace std;
namespace z2 {
class ServerProxy {
private:


public:

    virtual ~ServerProxy();

    virtual void sendMessageToServer(const shared_ptr<Message>& message) = 0;

};

}
#endif //Z2_SERVERPROXY_H
