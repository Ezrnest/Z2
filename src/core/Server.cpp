/*
 * Created by liyicheng on 2019/7/3.
 */

#include "Server.h"
#include "messages/PlayerMessage.h"

using namespace z2;

void Server::getMessage(const shared_ptr<Message> &command) {
    if (gameState != GameState::PAUSED) {
        return;
    }
    switch (command->getGeneralType()) {
        case GeneralMessageType::PlayerTurnFinish: {
            onPlayerTurnFinish(command);
            break;
        }
        case GeneralMessageType::PlayerDefeated:
            break;
        case GeneralMessageType::PlayerWin:
            break;
        case GeneralMessageType::UnitPerform:
        case GeneralMessageType::UnitBuy:
        case GeneralMessageType::UnitMove:
        case GeneralMessageType::UnitAttack:
            world->dealWithMessage(dynamic_pointer_cast<GameMessage>(command));
            break;
        case GeneralMessageType::StartGame:
        case GeneralMessageType::EndGame:
        case GeneralMessageType::PlayerTurnStart:
            throw logic_error("");

    }
}

bool z2::Server::registerClient(const shared_ptr<z2::Client> &client) {
    clients.push_back(client);
    return client->syncWorld(*world);
}

void z2::Server::startGame() {
    gameState = RUNNING;

    shared_ptr<Message> startGame(new ControlMessage(GeneralMessageType::StartGame));
    broadcastMessage(startGame);

    int playerId = world->getCurrentPlayer();
    callPlayer(playerId);
}

void z2::Server::pauseGame() {
    //TODO
}

void Server::sendMessage(const shared_ptr<Message> &message, int clientId) {
    clients[clientId]->sendMessage(message);
}

void Server::broadcastMessage(const shared_ptr<Message> &message) {
    for(auto &c : clients){
        c->sendMessage(message);
    }
}

void Server::callPlayer(int playerId) {
    shared_ptr<Message> msg(new PlayerMessage(GeneralMessageType::PlayerTurnStart, playerId));
    world->onPlayerTurnStart();
    sendMessage(msg, playerId);
}

void Server::shiftTurn() {
    world->onPlayerTurnFinish();
    callPlayer(world->nextPlayer());
}

void Server::onPlayerTurnFinish(const shared_ptr<Message> &message) {
    shared_ptr<PlayerMessage> msg = dynamic_pointer_cast<PlayerMessage>(message);
    int playerId = msg->getPlayerId();
    if (playerId != world->getCurrentPlayer()) {
        return;
    }
    shiftTurn();
}


z2::Server::~Server() = default;
