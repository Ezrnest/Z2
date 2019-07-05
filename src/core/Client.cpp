/*
 * Created by liyicheng on 2019/7/3.
 */

#include "Client.h"


int z2::Client::getClientId() const {
    return clientId;
}

void z2::Client::setClientId(int clientId_) {
    Client::clientId = clientId_;
}

z2::Client::~Client() = default;
