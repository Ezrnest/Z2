/*
 * Created by liyicheng on 2019/7/4.
 */

#include "CommandLineClient.h"

void z2::CommandLineClient::sendMessage(const std::shared_ptr<z2::Message> &command) {
    
}

bool z2::CommandLineClient::syncWorld(const z2::World &world) {
    //Always success
    return true;
}

const shared_ptr<z2::Server> &z2::CommandLineClient::getServer() const {
    return server;
}

void z2::CommandLineClient::setServer(const shared_ptr<z2::Server> &server) {
    CommandLineClient::server = server;
}
