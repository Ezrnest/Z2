/*
 * Created by liyicheng on 2019/7/6.
 */

#include "Client.h"

z2::Client::~Client() = default;

void z2::Client::sendMessageToServer(const shared_ptr<z2::Message> &message) {
    getServer()->sendMessageToServer(message);
}
