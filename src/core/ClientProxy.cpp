/*
 * Created by liyicheng on 2019/7/3.
 */

#include "ClientProxy.h"


int z2::ClientProxy::getClientId() const {
    return clientId;
}

void z2::ClientProxy::setClientId(int clientId_) {
    ClientProxy::clientId = clientId_;
}

z2::ClientProxy::~ClientProxy() = default;
