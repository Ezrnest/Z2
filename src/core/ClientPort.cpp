/*
 * Created by liyicheng on 2019/7/3.
 */

#include "ClientPort.h"


int z2::ClientPort::getClientId() const {
    return clientId;
}

void z2::ClientPort::setClientId(int clientId_) {
    ClientPort::clientId = clientId_;
}

z2::ClientPort::~ClientPort() = default;
