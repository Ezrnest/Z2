/*
 * Created by liyicheng on 2019/7/6.
 */

#include "LocalClient.h"

const shared_ptr<z2::Server> &z2::LocalClient::getRealServer() const {
    return server;
}

void z2::LocalClient::setRealServer(const shared_ptr<z2::Server> &server) {
    LocalClient::server = server;
}

void z2::LocalClient::setServerPort(const shared_ptr<z2::ServerPort> &server) {

}

shared_ptr<z2::ServerPort> z2::LocalClient::getServer() {
    return shared_ptr<ServerPort>();
}

void z2::LocalClient::setView(const shared_ptr<z2::GameGui> &gui) {
    LocalClient::gui = gui;
}

void z2::LocalClient::acceptMessage(const shared_ptr<z2::Message> &command) {
    //TODO

}

shared_ptr<z2::World> z2::LocalClient::getWorld() {
    return server->getWorld();
}

int z2::LocalClient::getPlayerId() {
    return playerId;
}

void z2::LocalClient::sendMessage(const shared_ptr<z2::Message> &command) {
    acceptMessage(command);
}

bool z2::LocalClient::syncWorld(const z2::World &world) {
    return true;
}

void z2::LocalClient::sendMessageToServer(const z2::MessagePtr &message) {
    server->acceptMessage(message);
}
