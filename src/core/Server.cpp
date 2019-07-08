/*
 * Created by liyicheng on 2019/7/3.
 */

#include "Server.h"
#include "Message.h"
#include "ClientPort.h"
#include "messages/PlayerMessage.h"
#include "messages/ControlMessage.h"
#include "../util/LogUtil.h"
using namespace z2;

void Server::acceptMessage(const shared_ptr<Message> &command) {
    if (gameState == GameState::PAUSED) {
        ancono::info("Reject message: Game Paused");
        return;
    }
//    ancono::info("Accepted message!");
    switch (command->getGeneralType()) {
        case GeneralMessageType::ControlMessage: {
            dealWithControlMessage(static_pointer_cast<ControlMessage>(command));
            break;
        }
        case GeneralMessageType::GameMessage: {
            world->dealWithMessage(static_pointer_cast<GameMessage>(command));
            break;
        }
        case GeneralMessageType::ChatMessage:
            break;
    }
}

bool z2::Server::registerClient(const shared_ptr<z2::ClientPort> &client) {
    client->setClientId(clients.size());

    clients.push_back(client);

    bool re =  client->syncWorld(*world);
    if(re){
        ancono::info("Client registered");
        return true;
    }else{
        ancono::warn("Client register failed!");
        return false;
    }
}

void z2::Server::startGame() {
    if(!checkGameReady()){
        return;
    }
    gameState = RUNNING;

    shared_ptr<Message> startGame(new ControlMessage(ControlMessageType ::StartGame));
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
    for (auto &c : clients) {
        c->sendMessage(message);
    }
}

void Server::callPlayer(int playerId) {
    shared_ptr<Message> msg(new PlayerMessage(ControlMessageType::PlayerTurnStart, playerId));
    world->onPlayerTurnStart();
    sendMessage(msg, playerId);
}

void Server::shiftTurn() {
    world->onPlayerTurnFinish();
    callPlayer(world->nextPlayer());
}

void Server::onPlayerTurnFinish(const shared_ptr<Message> &message) {
    shared_ptr<PlayerMessage> msg = static_pointer_cast<PlayerMessage>(message);
    int playerId = msg->getPlayerId();
    if (playerId != world->getCurrentPlayer()) {
        ancono::warn("Turn finish with not current player!");
        return;
    }
    shiftTurn();
}

const shared_ptr<World> &Server::getWorld() const {
    return world;
}

int Server::getGameState() const {
    return gameState;
}

void Server::setWorld(const shared_ptr<World> &world) {
    Server::world = world;
}

bool Server::checkGameReady() {
    if(gameState == RUNNING){
        ancono::warn("Already started!");
        return false;
    }
    if(clients.size() != world->getPlayerCount()){
        ancono::warn("Player count != client count");
        return false;
    }
    if(!world->checkReady()){
        ancono::warn("The world is not correctly loaded!");
        return false;
    }
    return true;
}

void Server::dealWithControlMessage(const shared_ptr<z2::ControlMessage>& message) {
    switch (message->getControlType()){
        case ControlMessageType::PlayerTurnFinish:{
            onPlayerTurnFinish(message);
            break;
        }
        case ControlMessageType::EndGame:{
            onEndGame();
            break;
        }
        case ControlMessageType::PlayerTurnStart:break;
        case ControlMessageType::PlayerDefeated:break;
        case ControlMessageType::PlayerWin:break;
        case ControlMessageType::StartGame:break;
    }
}

void Server::onEndGame() {
    ancono::info("Game ended!");
}


z2::Server::~Server() = default;
