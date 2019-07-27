//
// Created by liyicheng on 2019/7/3.
//

#include <cmath>
#include "World.h"
#include "../util/ArrayUtil.h"
#include "../core/Message.h"
#include "config/EntityRepository.h"
#include "config/SerializableRegistry.h"
#include "../entity/MeleeUnit.h"
#include "../entity/RangeUnit.h"
#include "../core/messages/GameMessage.h"
#include "../core/messages/UnitBuy.h"
#include "../core/messages/UnitMove.h"
#include "../core/messages/UnitAttack.h"
#include "../core/messages/EntityPerform.h"
#include "../core/messages/EntityKill.h"
#include "plog/Log.h"

namespace z2 {
using EntityPtr = shared_ptr<Entity>;
using namespace ancono;


World::World(int width_, int height_, int playerCount)
        : width(width_), height(height_),
          players(playerCount), playerCount(playerCount) {
    data = initMatrix<Tile>(width_, height_);
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            Tile &t = data[i][j];
            t.setPlayerCount(playerCount);

        }
    }
    //init players
    for (int i = 0; i < playerCount; i++) {
        players[i].setPlayerId(i);
        players[i].setGroupId(i);
    }
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

const string &World::className() {
    static string name = "World";
    return name;
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
    if (!isInside(point)) {
        return shared_ptr<Entity>();
    }
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
    int count = 0;
    do {
        currentPlayer++;
        if (count > getPlayerCount()) {
            PLOG_WARNING << "All players are dead!";
            return -1;
        }
        count++;
        if (currentPlayer >= getPlayerCount()) {
            currentPlayer -= getPlayerCount();
        }
    } while (players[currentPlayer].isDead());
    return currentPlayer;
}

void World::initPlainDataFrom(const World &world) {
    height = world.height;
    width = world.width;
    players = world.players;
    objectUID = world.objectUID;
    currentPlayer = world.currentPlayer;
    playerCount = world.playerCount;
    entityMap = world.entityMap;
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
        case GameMessageType::EntityPerform: {
            const shared_ptr<EntityPerform> msg = static_pointer_cast<EntityPerform>(message);
            performEntity(msg->getPos());
            break;
        }
        case GameMessageType::UnitAttack: {
            const shared_ptr<UnitAttack> msg = static_pointer_cast<UnitAttack>(message);
            attackEntity(msg->getFrom(), msg->getDest());
            break;
        }
        case GameMessageType::EntityKill: {
            const shared_ptr<EntityKill> msg = static_pointer_cast<EntityKill>(message);
            removeEntity(msg->getPos());
            break;
        }
    }
}

bool World::moveEntity(const Point &from, const Point &dest) {
    if (!isInside(from) || !isInside(dest)) {
        PLOG_WARNING << "Attempting to move outside the map!";
        return false;
    }
    Tile &destTile = getTile(dest);
    if (destTile.isOccupied()) {
        //TODO add log info
        PLOG_WARNING << "Attempting to move to an occupied tile!";
        return false;
    }
    Tile &fromTile = getTile(from);
    shared_ptr<GameUnit> entity = dynamic_pointer_cast<GameUnit>(fromTile.getEntity());
    if (!entity) {
        // not actually entity
        PLOG_WARNING << "Attempting to move a non-entity!";
        return false;
    }
    int requiredMoves = pathLength(from, dest);
    if (!entity->requireMoves(requiredMoves)) {
        PLOG_WARNING << "Unit out of move!";
        return false;
    }
    destTile.setEntity(fromTile.removeEntity());

    onEntityMoved(from, dest, entity);

    return true;
}

void World::buyEntity(int playerId, const Point &pos, const string &entityName) {
    const Point &whereToPlace = getAdjacentEmptyPos(pos);
    if (whereToPlace.x < 0) {
        PLOG_WARNING << "Nowhere to place the bought unit.";
        return;
    }
    if (playerId != Player::NO_PLAYER) {
        Player &p = players[playerId];
        auto &repo = EntityRepository::instance();
        if (!repo.hasEntity(entityName)) {
            PLOG_WARNING << ("Unknown entity: " + entityName);
            return;
        }
        auto &info = repo.getEntityInfo(entityName);
        int cost = info.getProperties().getInt("price", 25);
        if (!p.requireGold(cost)) {
            PLOG_WARNING << "Not enough gold!";
            return;
        }
    }

    createEntity(whereToPlace, entityName, playerId);

}

bool World::performMeleeAttack(const Point &from, const Point &dest, const shared_ptr<MeleeUnit> &melee,
                               const shared_ptr<EntityWithHealth> &victim) {
    int damage = melee->getAttackStrength();
    onEntityDamaging(from, dest, melee, victim, damage);
    victim->dealDamage(damage);
    if (victim->isDead()) {
        removeEntity(dest);
        return true;
    }
    return false;
}


void World::attackEntityMelee(const Point &from, const Point &dest, const shared_ptr<MeleeUnit> &melee,
                              const shared_ptr<EntityWithHealth> &victim) {
    if (!from.isAdjacentTo(dest)) {
        PLOG_WARNING << "Not adjacent for melee attack!";
        return;
    }

    if (performMeleeAttack(from, dest, melee, victim)) {
        //occupy the position
        Tile &destTile = getTile(dest);
        Tile &fromTile = getTile(from);
        destTile.setEntity(fromTile.removeEntity());
        onEntityMoved(from, dest, melee);
    } else {
        const shared_ptr<MeleeUnit> avenger = dynamic_pointer_cast<MeleeUnit>(victim);
        //attack back
        if (bool(avenger)) {
            performMeleeAttack(dest, from, avenger, melee);
        }
    }
}

void World::attackEntityRange(const Point &from, const Point &dest, const shared_ptr<RangeUnit> &range,
                              const shared_ptr<EntityWithHealth> &victim) {
    int damage = range->getAttackStrength();
    onEntityDamaging(from, dest, range, victim, damage);
    victim->dealDamage(damage);
    if (victim->isDead()) {
        removeEntity(dest);
    }
}

void World::attackEntity(const Point &from, const Point &dest) {
    const EntityPtr &attacker = getEntity(from);
    const EntityPtr &receiver = getEntity(dest);
    if (!bool(attacker) || !bool(receiver)) {
        PLOG_WARNING << "No unit to make attack!";
        return;
    }
    if (!attacker->requireRestMoves()) {
        PLOG_WARNING << "No sufficient move to perform attack!";
        return;
    }
    if (isOfSameGroup(attacker, receiver)) {
        PLOG_WARNING << "Cannot attack an ally!";
        return;
    }
    const shared_ptr<EntityWithHealth> victim = dynamic_pointer_cast<EntityWithHealth>(receiver);
    if (!victim) {
        PLOG_WARNING << "Not a valid target!";
        return;
    }
    const shared_ptr<MeleeUnit> melee = dynamic_pointer_cast<MeleeUnit>(attacker);
    if (melee) {
        attackEntityMelee(from, dest, melee, victim);
        return;
    }
    const shared_ptr<RangeUnit> range = dynamic_pointer_cast<RangeUnit>(attacker);
    if (range) {
        attackEntityRange(from, dest, range, victim);
        return;
    }
    PLOG_WARNING << "The unit can't attack!";
}


void World::performEntity(const Point &target) {
    const EntityPtr &entity = getEntity(target);
    if (!entity) {
        PLOG_WARNING << "No unit to perform!";
        return;
    }
    if (!entity->requireRestMoves()) {
        PLOG_WARNING << "No sufficient moves!";
        return;
    }
    entity->performAbility(target, *this);
    onEntityPerformed(target, entity);
}


void World::onEntityMoved(const Point &from, const Point &dest, const shared_ptr<GameUnit> &entity) {
    //TODO add log info
    //TODO dispatch

    PLOG(plog::info) << "Entity moved!";
    updateVisibility(entity->getOwnerId());
}

void
World::onEntityCreated(const Point &pos, const shared_ptr<Entity> &entity, const string &entityName, int playerId) {
    //TODO dispatch
    PLOG(plog::info) << "Entity [" + entityName + "] created!";
    setEntityVisibility(pos.x, pos.y, entity, playerId);
}

void World::onEntityDamaging(const Point &from, const Point &dest,
                             const shared_ptr<Entity> &attacker,
                             const shared_ptr<EntityWithHealth> &receiver, int &damage) {
    PLOG(plog::info) << "Entity damaged!";
}


void World::onEntityRemoved(const Point &pos, const shared_ptr<Entity> &entity) {
    PLOG(plog::info) << "Entity [" + entity->getEntityName() + "] removed";
    int playerId = entity->getOwnerId();
    updateVisibility(playerId);
    if(checkPlayerLostAllUnit(playerId)){
        onPlayerDefeated(playerId);
    }
}

void World::onEntityPerformed(const Point &pos, const EntityPtr &entity) {
    PLOG(plog::info) << "Entity [" + entity->getEntityName() + "] performed!";
}


void World::onPlayerTurnStart() {
    PLOG(plog::info) << "Player turn started: "<< currentPlayer;
    updateVisibility(currentPlayer);
    refreshMoves(currentPlayer);
}

void World::onPlayerTurnStart(int playerId) {
    currentPlayer = playerId;
    onPlayerTurnStart();
}

void World::onPlayerTurnFinish() {
    PLOG(plog::info) << "Player turn finished.";
    //TODO: update visibility
}

void World::onPlayerDefeated(int playerId) {
    players[playerId].setDead(true);
    PLOG(plog::info) << "Player" << playerId << " is defeated";
    checkPlayerGroupWin();
}

void World::onPlayerGroupWon(int groupId) {
    PLOG(plog::info) << "Player group " << groupId << " won!";
}



shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId, int playerId) {
    Tile &tile = getTile(pos);
    auto entity = EntityRepository::instance().createEntity(entityId, getNextObjectId());
    if (entity == nullptr) {
        PLOG_WARNING << "Unable to create entity named: " << entityId;
        return shared_ptr<Entity>();
    }
    tile.setEntity(entity);
    entityMap.insert(make_pair(entity->getObjectId(), entity));
    entity->setOwnerId(playerId);
    onEntityCreated(pos, entity, entityId, playerId);
    return entity;
}

shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId) {
    return createEntity(pos, entityId, Player::NO_PLAYER);
}

void World::removeEntity(const Point &pos) {
    shared_ptr<Entity> entity = getTile(pos).removeEntity();
    entityMap.erase(entity->getObjectId());
    onEntityRemoved(pos, entity);
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

void World::forEachEntitiesOf(int playerId, const function<void(int, int, shared_ptr<Entity>&)> &f) {
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
    int xR = min(x + range, width - 1);
    int yR = min(y + range, height - 1);
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

void World::serializeTo(ostream &output) {
    //general information
    output << className() << ' ';
    output << width << ' '
           << height << ' '
           << playerCount << ' ';
    // players
    output << currentPlayer << ' ';
    for (Player &p : players) {
        p.saveDataTo(output);
    }
    //objects
    output << objectUID << ' ';
    output << entityMap.size() << ' ';
    for (auto &en : entityMap) {
        output << en.first << ' ';
        en.second->serializeTo(output);
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            saveTileData(data[i][j], output);
        }
    }

}


World *World::loadFrom(istream &input) {
    int width, height, playerCount;
    input >> width >> height >> playerCount;
    auto *world = new World(width, height, playerCount);
    input >> world->currentPlayer;
    for (int i = 0; i < playerCount; i++) {
        world->getPlayer(i).loadDataFrom(input);
    }

    input >> world->objectUID;
    int objectCount;
    input >> objectCount;
    auto &sr = SerializableRegistry::instance();
    for (int i = 0; i < objectCount; i++) {
        int uid;
        input >> uid;
        Serializable *s = sr.deserialize(input);
        shared_ptr<Entity> en(static_cast<Entity *>(s));
        world->entityMap.insert(make_pair(uid, en));
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            world->loadTileData(world->data[i][j], input);
        }
    }

    return world;
}


void World::saveTileData(Tile &t, ostream &out) {
    out << static_cast<int>(t.terrain) << ' '
        << static_cast<int>(t.resource) << ' ';
    if (t.hasEntity()) {
        out << t.entity->getObjectId();
    } else {
        out << -1;
    }
    out << ' ';

    for (int i = 0; i < playerCount; i++) {
        int vi = static_cast<int>(t.getVisibility(i));
        out << vi << ' ';
    }
}

void World::loadTileData(Tile &t, istream &input) {
    int terrain, resource;
    input >> terrain >> resource;
    t.terrain = static_cast<Terrain>(terrain);
    t.resource = static_cast<Resource>(resource);
    int uid;
    input >> uid;
    if (uid > 0) {
        t.setEntity(entityMap[uid]);
    }
    for (int i = 0; i < playerCount; i++) {
        int v;
        input >> v;
        t.visibility[i] = static_cast<Visibility>(v);
    }
}

const string &World::getClassName() const {
    return className();
}

Point World::searchFor(int playerId, const string &entityName) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (!data[i][j].hasEntity()) {
                continue;
            }
            const shared_ptr<Entity> &en = data[i][j].getEntity();
            if (en->getEntityName() == entityName) {
                return Point(i, j);
            }
        }
    }
    return {-1, -1};
}

bool World::isOfSameGroup(const shared_ptr<Entity> &e1, const shared_ptr<Entity> &e2) {
    int p1 = e1->getOwnerId();
    int p2 = e2->getOwnerId();
    if (p1 == Player::NO_PLAYER || p2 == Player::NO_PLAYER) {
        return false;
    }
    int g1 = players[p1].getGroupId();
    int g2 = players[p2].getGroupId();
    return !(g1 == Player::NO_GROUP || g2 == Player::NO_GROUP || g1 != g2);
}

bool World::checkPlayerLostAllUnit(int playerId) {
    bool lostAll = true;
    forEachEntitiesOf(playerId,
            [&lostAll](int, int, shared_ptr<Entity> &) { lostAll = false; });
    return lostAll;
}

void World::checkPlayerGroupWin() {
    int group = Player::NO_GROUP;
    for(Player& p : players){
        if(p.isDead()){
            continue;
        }
        if (group == -1) {
            group = p.getGroupId();
        } else if (group != p.getGroupId()) {
            return;
        }
    }
    onPlayerGroupWon(group);
}




}