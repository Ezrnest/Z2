//
// Created by liyicheng on 2019/7/3.
//

#include "World.h"
#include "../util/ArrayUtil.h"
#include "../util/LogUtil.h"
#include "../core/Message.h"
#include "../core/EntityRepository.h"
#include "../core/messages/GameMessage.h"

using namespace z2;


World::World(int width_, int height_) : width(width_), height(height_) {
    data = initMatrix<Tile>(width_, height_);
}

World::World(const World &world) {
    data = copyMatrix(world.data, world.height, world.width);
    initPlainDataFrom(world);
}

World &World::operator=(const World &world) {
    auto dataOrigin = data;
    data = copyMatrix(world.data, world.height, world.width);
    deleteMatrix(dataOrigin, height);
    initPlainDataFrom(world);

    return *this;
}

World::World(World &&world) noexcept {
    data = world.data;
    world.data = nullptr;
    initPlainDataFrom(world);
}

World &World::operator=(World &&world) noexcept {
    deleteMatrix(data, height);

    data = world.data;
    initPlainDataFrom(world);

    world.data = nullptr;

    return *this;
}

World::~World() {
    deleteMatrix(data, height);
}

bool World::isInside(int x, int y) const {
    return 0 <= x < width && 0 <= y < height;
}

bool World::isOccupied(int x, int y) const {
    return getTile(x, y).isOccupied();
}

Tile &World::getTile(int x, int y) const {
    return data[x][y];
}

Tile &World::getTile(const Point &pos) const {
    return getTile(pos.x, pos.y);
}


const vector<Player> &World::getPlayers() const {
    return players;
}

int World::getPlayerCount() const {
    return players.size();
}

void World::setPlayers(const vector<Player> &players_) {
    World::players = players_;
}

int World::getNextObjectId() {
    return objectUID++;
}

int World::getCurrentPlayer() const {
    return currentPlayer;
}

int World::nextPlayer() {
    currentPlayer++;
    if (currentPlayer >= getPlayerCount()) {
        currentPlayer -= getPlayerCount();
    }
    return currentPlayer;
}

void World::initPlainDataFrom(const World &world) {
    height = world.height;
    width = world.width;
    players = world.players;
    objectUID = world.objectUID;
    currentPlayer = world.currentPlayer;
}

void World::dealWithMessage(const shared_ptr<GameMessage> &message) {
    //TODO
    switch (message->getGameType()){

        case GameMessageType::UnitBuy:break;
        case GameMessageType::UnitMove:break;
        case GameMessageType::UnitPerform:break;
        case GameMessageType::UnitAttack:break;
    }
}

bool World::moveEntity(const Point &from, const Point &dest) {
    auto destTile = getTile(dest);
    if (destTile.isOccupied()) {
        //TODO add log info
        return false;
    }
    auto fromTile = getTile(from);
    shared_ptr<GameUnit> entity = dynamic_pointer_cast<GameUnit>(fromTile.getEntity());
    if (!entity) {
        // not actually entity
        return false;
    }
    destTile.setEntity(fromTile.removeEntity());
    onEntityMoved(from, dest, entity);

    return true;
}

void World::onEntityMoved(const Point &from, const Point &dest, const shared_ptr<GameUnit> &entity) {
    //TODO add log info
}

void World::onPlayerTurnStart() {
    info("Player turn started.");
}

void World::onPlayerTurnFinish() {
    info("Player turn finished.");
}

shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId, const Properties &prop) {
    auto &tile = getTile(pos);
    auto entity = EntityRepository::instance().createEntity<Entity>(entityId, getNextObjectId(), prop);
    tile.setEntity(entity);
    return entity;
}

shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId) {
    return createEntity(pos, entityId, Properties());
}

Player &World::getPlayer(int playerId) {
    return players[playerId];
}








