//
// Created by liyicheng on 2019/7/3.
//

#include <cmath>
#include "World.h"
#include "../util/ArrayUtil.h"
#include "../util/LogUtil.h"
#include "../core/Message.h"
#include "../core/EntityRepository.h"
#include "../core/messages/GameMessage.h"
#include "../core/messages/UnitMove.h"
#include "../core/messages/UnitBuy.h"

namespace z2 {

using namespace ancono;

World::World(int width_, int height_, int playerCount)
        : width(width_), height(height_),
          players(playerCount), playerCount(playerCount) {
    data = initMatrix<Tile>(width_, height_);
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            Tile& t = data[i][j];
            data[i][j].setPlayerCount(playerCount);

        }
    }
    //init players
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


bool World::isInside(const Point &pos) const {
    return isInside(pos.x, pos.y);
}


bool World::isOccupied(int x, int y) const {
    return getTile(x, y).isOccupied();
}

int World::getWidth() const {
    return width;
}

int World::getHeight() const {
    return height;
}

Tile &World::getTile(int x, int y) const {
    return data[x][y];
}

Tile &World::getTile(const Point &pos) const {
    return getTile(pos.x, pos.y);
}

const shared_ptr<Entity> World::getEntity(const Point &point) {
    return getTile(point).getEntity();
}


const vector<Player> &World::getPlayers() const {
    return players;
}

int World::getPlayerCount() const {
    return playerCount;
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
    playerCount = world.playerCount;
}

void World::dealWithMessage(const shared_ptr<GameMessage> &message) {
    //TODO
    switch (message->getGameType()) {
        case GameMessageType::UnitBuy: {
            const shared_ptr<UnitBuy> msg = static_pointer_cast<UnitBuy>(message);
            buyEntity(msg->getPlayerId(), msg->getPos(), msg->getIdentifier());
            break;
        }
        case GameMessageType::UnitMove: {
            const shared_ptr<UnitMove> msg = static_pointer_cast<UnitMove>(message);
            moveEntity(msg->getFrom(), msg->getDest());
            break;
        }
        case GameMessageType::UnitPerform:
            break;
        case GameMessageType::UnitAttack:
            break;
    }
}

bool World::moveEntity(const Point &from, const Point &dest) {
    if (!isInside(from) || !isInside(dest)) {
        warn("Attempting to move outside the map!");
        return false;
    }
    Tile& destTile = getTile(dest);
    if (destTile.isOccupied()) {
        //TODO add log info
        warn("Attempting to move to an occupied tile!");
        return false;
    }
    Tile& fromTile = getTile(from);
    shared_ptr<GameUnit> entity = dynamic_pointer_cast<GameUnit>(fromTile.getEntity());
    if (!entity) {
        // not actually entity
        warn("Attempting to move a non-entity!");
        return false;
    }
    int requiredMoves = pathLength(from, dest);
    if (requiredMoves > entity->getRemainingMoves()) {
        warn("Unit out of move!");
        return false;
    }
    destTile.setEntity(fromTile.removeEntity());
    entity->decreaseMoves(requiredMoves);
    onEntityMoved(from, dest, entity);

    return true;
}

void World::buyEntity(int playerId, const Point &pos, const string &entityName) {
    const Point &whereToPlace = getAdjacentEmptyPos(pos);
    if (whereToPlace.x < 0) {
        warn("Nowhere to place the bought unit.");
        return;
    }
    createEntity(whereToPlace, entityName, playerId);

}


void World::onEntityMoved(const Point &from, const Point &dest, const shared_ptr<GameUnit> &entity) {
    //TODO add log info
    //TODO dispatch

    info("Entity moved!");
    updateVisibility(entity->getOwnerId());
}

void World::onEntityCreated(const Point &pos, const string &entityName, int playerId) {
    //TODO dispatch
    info("Entity [" + entityName + "] created!");
    setEntityVisibility(pos.x, pos.y, getEntity(pos), playerId);
}


void World::onPlayerTurnStart() {
    info("Player turn started.");
    updateVisibility(currentPlayer);
    refreshMoves(currentPlayer);
}

void World::onPlayerTurnFinish() {
    info("Player turn finished.");
    //TODO: update visibility
}

shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId, int playerId) {
    Tile &tile = getTile(pos);
    auto entity = EntityRepository::instance().createEntity(entityId, getNextObjectId());
    tile.setEntity(entity);
    entity->setOwnerId(playerId);
    onEntityCreated(pos, entityId, playerId);
    return entity;
}

shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId) {
    return createEntity(pos, entityId, Player::NO_PLAYER);
}

Player &World::getPlayer(int playerId) {
    return players[playerId];
}

Tile *World::getAdjacentEmptyTile(const Point &pos) const {
    for (const Point &dir : Point::directions()) {
        auto p = pos + dir;
        if (!isInside(p)) {
            continue;
        }
        Tile &tile = getTile(p);
        if (!tile.isOccupied()) {
            return &tile;
        }
    }
    return nullptr;
}

Point World::getAdjacentEmptyPos(const Point &pos) const {
    for (const Point &dir : Point::directions()) {
        auto p = pos + dir;
        if (!isInside(p)) {
            continue;
        }
        Tile &tile = getTile(p);
        if (!tile.isOccupied()) {
            return p;
        }
    }
    return {-1, -1};
}

void World::resetVisibility(int playerId) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            data[i][j].resetVisibility(playerId);
        }
    }
}

void World::forEachEntitiesOf(int playerId, const function<void(int, int, shared_ptr<Entity>)> &f) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Tile &t = data[i][j];
            if (!t.hasEntity()) {
                continue;
            }
            auto entity = t.getEntity();
            if (entity->getOwnerId() != playerId) {
                continue;
            }
            f(i, j, entity);
        }
    }
}

void World::updateVisibility(int playerId) {
    if (playerId < 0) {
        return;
    }
    resetVisibility(playerId);
    forEachEntitiesOf(playerId,
                      [this, playerId](int i, int j, const shared_ptr<Entity> &entity) {
                          setEntityVisibility(i, j, entity, playerId);
                      });
}

void World::setEntityVisibility(int x, int y, const shared_ptr<Entity> &en, int playerId) {
    int range = en->getVisibility();
    int xL = max(x - range, 0);
    int yL = max(y - range, 0);
    int xR = min(x + range, width-1);
    int yR = min(y + range, height-1);
    for (int i = xL; i <= xR; ++i) {
        for (int j = yL; j <= yR; j++) {
            if (hypot(i - x, j - y) <= range) {
                data[i][j].setVisibility(playerId, Visibility::CLEAR);
            }
        }
    }
}


void World::refreshMoves(int playerId) {
    forEachEntitiesOf(playerId,
                      [this, playerId](int i, int j, const shared_ptr<Entity> &entity) {
                          entity->refreshMoves();
                      });
}


bool World::checkReady() {
    if (players.size() != playerCount) {
        return false;
    }
    // other stuffs
    return true;
}

int World::pathLength(const Point &start, const Point &dest) const {
//    int** length
    return start.girdDistance(dest);
}


}