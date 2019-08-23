//
// Created by liyicheng on 2019/7/3.
//

#include <cmath>
#include <stack>
#include <event/EntityEvent.h>
#include <core/messages/TechResearch.h>
#include <config/TechRepository.h>
#include <event/InGamePlayerEvent.h>
#include <core/messages/SetPlayerData.h>
#include <event/PlayersWon.h>
#include <entity/ConstructionBase.h>
#include "event/GameEvent.h"
#include "event/StateEvent.h"
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
    data = copyMatrix(world.data, world.width, world.height);
    initPlainDataFrom(world);
}

World &World::operator=(const World &world) {
    auto dataOrigin = data;
    data = copyMatrix(world.data, world.width, world.height);
    deleteMatrix(dataOrigin, height);
    initPlainDataFrom(world);

    return *this;
}

World::World(World &&world) noexcept {
    data = world.data;
    world.data = nullptr;
    distanceMap = world.distanceMap;
    world.distanceMap = nullptr;
    initPlainDataFrom(world);
}

World &World::operator=(World &&world) noexcept {
    deleteMatrix(data, width);

    data = world.data;
    distanceMap = world.distanceMap;
    initPlainDataFrom(world);

    world.distanceMap = nullptr;
    world.data = nullptr;

    return *this;
}

World::~World() {
    deleteMatrix(data, width);
    if (distanceMap != nullptr) {
        deleteMatrix(distanceMap, width);
    }
}

const string &World::className() {
    static string name = "World";
    return name;
}


bool World::isInside(int x, int y) const {
    return 0 <= x && x < width && 0 <= y && y < height;
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

shared_ptr<Entity> World::getEntity(const Point &point) {
    if (!isInside(point)) {
        return shared_ptr<Entity>();
    }
    return getTile(point).getEntity();
}

shared_ptr<Entity> World::getEntity(unsigned int entityId){
    auto it = entityMap.find(entityId);
    if (it == entityMap.end()) {
        return shared_ptr<Entity>();
    }
    return it->second;
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
    if (currentPlayer < -1) {
        currentPlayer = -1;
    }
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

int World::nextPlayerFromCurrent() {
    currentPlayer--;
    return nextPlayer();
}


void World::initPlainDataFrom(const World &world) {
    mapName = world.mapName;
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
        case GameMessageType::TechResearch: {
            const shared_ptr<TechResearch> msg = static_pointer_cast<TechResearch>(message);
            researchTechnology(msg->getPlayerId(), msg->getTechId());
            break;
        }
        case GameMessageType::SetPlayerData: {
            const shared_ptr<SetPlayerData> msg = static_pointer_cast<SetPlayerData>(message);
            msg->setDataForWorld(*this);
            break;
        }

    }
}

/* ------------------------------- Start of Game Operation -------------------------------------- */
bool playerCanControl(int playerId, Entity &en) {
    return playerId == Player::NO_PLAYER || en.getOwnerId() == playerId;
}

bool World::canMove(const Point &from, const Point &dest, int playerId) {
    if (!isInside(from) || !isInside(dest)) {
        return false;
    }
    Tile &destTile = getTile(dest);
    if (destTile.isOccupied()) {
        return false;
    }
    Tile &fromTile = getTile(from);
    shared_ptr<GameUnit> entity = dynamic_pointer_cast<GameUnit>(fromTile.getEntity());
    if (!entity) {
        return false;
    }
    if (!playerCanControl(playerId, *entity)) {
        return false;
    }
    int requiredMoves = pathLength(from, dest, entity);
    return entity->getRemainingMoves() >= requiredMoves;
}

bool World::moveEntity(const Point &from, const Point &dest) {
    if (!isInside(from) || !isInside(dest)) {
        PLOG_WARNING << "Attempting to move outside the map!";
        return false;
    }
    Tile &destTile = getTile(dest);
    if (destTile.isOccupied()) {
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
    int requiredMoves = pathLength(from, dest, entity);
    if (!entity->requireMoves(requiredMoves)) {
        PLOG_WARNING << "Unit out of move!";
        return false;
    }
    destTile.setEntity(fromTile.removeEntity());

    onEntityMoved(from, dest, entity);

    return true;
}

bool World::canBuy(int playerId, const Point &pos, const string &entityName) {
    const Point &whereToPlace = getAdjacentEmptyPos(pos);
    if (whereToPlace.x < 0) {
        return false;
    }
    if (playerId != Player::NO_PLAYER) {
        Player &p = players[playerId];
        auto &repo = EntityRepository::instance();
        if (!repo.hasEntity(entityName)) {
            return false;
        }
        auto &info = repo.getEntityInfo(entityName);
        if (!info.isBuyableByPlayer(p)) {
            return false;
        }
        int cost = info.getProperties().getInt("price", 0);
        if (p.getGold() < cost) {
            return false;
        }
        auto en = dynamic_pointer_cast<ConstructionBase>(getEntity(pos));
        if(!en || en->getOwnerId() != playerId){
            return false;
        }
    }
    return true;
}

void World::buyEntity(int playerId, const Point &pos, const string &entityName) {
//    const Point &whereToPlace = getAdjacentEmptyPos(pos);
//    if (whereToPlace.x < 0) {
//        PLOG_WARNING << "Nowhere to place the bought unit.";
//        return;
//    }
//    if (playerId != Player::NO_PLAYER) {
//        Player &p = players[playerId];
//        auto &repo = EntityRepository::instance();
//        if (!repo.hasEntity(entityName)) {
//            PLOG_WARNING << ("Unknown entity: " + entityName);
//            return;
//        }
//        auto &info = repo.getEntityInfo(entityName);
//        if (!info.isBuyableByPlayer(p)) {
//            PLOG_WARNING << "Entity: " << entityName << " is not buyable for player " << p.getPlayerId();
//            return;
//        }
//        int cost = info.getProperties().getInt("price", 0);
//        if (!p.requireGold(cost)) {
//            PLOG_WARNING << "Not enough gold!";
//            return;
//        }
//    }
    if (!canBuy(playerId, pos, entityName)) {
        PLOG_WARNING << "Entity: " << entityName << " is not buyable for player " << playerId;
        return;
    }
    auto en = dynamic_pointer_cast<ConstructionBase>(getEntity(pos));
    if(!en){
        return;
    }
    en->buyEntity(pos, entityName, *this);
//    createEntity(whereToPlace, entityName, playerId);

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

bool World::canAttack(const Point &from, const Point &dest, int playerId) {
    const EntityPtr &attacker = getEntity(from);
    const EntityPtr &receiver = getEntity(dest);
    if (!bool(attacker) || !bool(receiver)) {
        return false;
    }
    if (attacker->getRemainingMoves() < 1) {
        return false;
    }
    if (!playerCanControl(playerId, *attacker)) {
        return false;
    }
    if (isOfSameGroup(attacker, receiver)) {
        return false;
    }
    const shared_ptr<EntityWithHealth> victim = dynamic_pointer_cast<EntityWithHealth>(receiver);
    if (!victim) {
        return false;
    }
    const shared_ptr<MeleeUnit> melee = dynamic_pointer_cast<MeleeUnit>(attacker);
    if (melee) {
        return from.isAdjacentTo(dest);
    }
    const shared_ptr<RangeUnit> range = dynamic_pointer_cast<RangeUnit>(attacker);
    if (range) {
        double r = range->computeRangeAt(from, *this);
        return r * r >= from.distanceSq(dest); // Required in range
    }
    return false;
}


void World::attackEntityMelee(const Point &from, const Point &dest, const shared_ptr<MeleeUnit> &melee,
                              const shared_ptr<EntityWithHealth> &victim) {
    if (!from.isAdjacentTo(dest)) {
        PLOG_WARNING << "Not adjacent for melee attack!";
        return;
    }
    melee->requireRestMoves();
    if (performMeleeAttack(from, dest, melee, victim)) {
        //occupy the position
        Tile &fromTile = getTile(from);
        Tile &destTile = getTile(dest);
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
    double r = range->computeRangeAt(from, *this);
    if (r * r < from.distanceSq(dest)) {
        PLOG_WARNING << "Out of range for range attack!";
        return;
    }
    range->requireRestMoves();
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
    if (attacker->getRemainingMoves() < 1) {
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

bool World::canPerform(const z2::Point &target, int playerId) {
    auto en = getEntity(target);
    if (!en) {
        return false;
    }
    if (!playerCanControl(playerId, *en)) {
        return false;
    }
    if (en->getRemainingMoves() < 1) {
        return false;
    }
    return en->canPerformAbility(target, *this);
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

bool World::canResearchTechnology(int playerId, const string &techId) {
    if (!isValidLivingPlayer(playerId)) {
        return false;
    }
    auto &repo = TechRepository::instance();
    if (!repo.containsTech(techId)) {
        return false;
    }
    Player &p = getPlayer(playerId);
    const Technology &tech = repo.getTechnology(techId);
    if (!tech.isResearchable(p)) {
        return false;
    }
    if (p.getGold() < tech.getPrice()) {
        return false;
    }
    return p.getTechPoints() >= 1;
}

bool World::researchTechnology(int playerId, const string &techId) {
//    if(!isValidLivingPlayer(playerId)){
//        PLOG_WARNING << "Invalid playerId: " << playerId;
//        return false;
//    }

//    if (!repo.containsTech(techId)) {
//        PLOG_WARNING << "Invalid techId: " << techId;
//        return false;
//    }

//    if(!tech.isResearchable(p)){
//        PLOG_WARNING << "Player " << playerId << " can't research tech "<< techId;
//        return false;
//    }
//    if (!p.requireGold(tech.getPrice())) {
//        PLOG_WARNING << "No sufficient gold to research!";
//        return false;
//    }
    if (!canResearchTechnology(playerId, techId)) {
        PLOG_WARNING << "Player " << playerId << " can't research tech " << techId;
        return false;
    }
    auto &repo = TechRepository::instance();
    Player &p = getPlayer(playerId);
    const Technology &tech = repo.getTechnology(techId);

    p.consumeTechPoint();
    p.requireGold(tech.getPrice());

    p.addTech(tech.getId());
    onPlayerResearchedTech(playerId, techId);
    return true;
}

/* ------------------------------- End of Game Operation -------------------------------------- */

/* -------------------------------------- Start of on... ---------------------------------------------- */


void
World::onEntityCreated(const Point &pos, const shared_ptr<Entity> &entity, const string &entityName, int playerId) {
    PLOG(plog::info) << "[World] Entity [" + entityName + "] created!";
    setEntityVisibility(pos.x, pos.y, entity, playerId);
    shared_ptr<GameEvent> event(new EntityEvent(entity, EntityEventType::EntityCreated));
    dispatcher.publish(event);
}

void World::onEntityMoved(const Point &from, const Point &dest, const shared_ptr<GameUnit> &entity) {
    PLOG(plog::info) << "[World] Entity moved!";
    entity->setPos(dest);

    updateVisibility(entity->getOwnerId());
    shared_ptr<GameEvent> event(new EEntityMoved(entity, from, dest));
    dispatcher.publish(event);
}

void World::onEntityDamaging(const Point &from, const Point &dest,
                             const shared_ptr<Entity> &attacker,
                             const shared_ptr<EntityWithHealth> &receiver, int &damage) {
    PLOG(plog::info) << "[World] Entity damaged!";
    auto t = new EEntityDamaging(receiver, attacker, damage);
    shared_ptr<GameEvent> event(t);
    dispatcher.publish(event);
    damage = t->getDamage();
}


void World::onEntityRemoved(const Point &pos, const shared_ptr<Entity> &entity) {
    PLOG(plog::info) << "[World] Entity [" + entity->getEntityName() + "] removed";
    shared_ptr<GameEvent> event(new EntityEvent(entity, EntityEventType::EntityRemoved));
    dispatcher.publish(event);

    int playerId = entity->getOwnerId();
    updateVisibility(playerId);
    if (checkPlayerLostAllUnit(playerId)) {
        onPlayerDefeated(playerId);
    }
}

void World::onEntityPerformed(const Point &pos, const EntityPtr &entity) {
    PLOG(plog::info) << "[World] Entity [" + entity->getEntityName() + "] performed!";
    shared_ptr<GameEvent> event(new EntityEvent(entity, EntityEventType::EntityPerformed));
    dispatcher.publish(event);
}

void World::onPlayerResearchedTech(int playerId, const string &techId) {
    PLOG_INFO << "[World] Player " << playerId << " reserached tech " << techId;
    GameEventPtr event(new TechResearchedEvent(playerId, techId));
    dispatcher.publish(event);
}


void World::onPlayerTurnStart() {
    PLOG(plog::info) << "Player turn started: " << currentPlayer;
    updateVisibility(currentPlayer);
    refreshMoves(currentPlayer);
    getCurrentAsPlayer().refreshTechPoints();
    shared_ptr<GameEvent> event(new PlayerEvent(StateEventType::PlayerTurnStarted, currentPlayer));
    dispatcher.publish(event);
}

void World::onPlayerTurnStart(int playerId) {
    currentPlayer = playerId;
    onPlayerTurnStart();
}

void World::onPlayerTurnFinish() {
    PLOG(plog::info) << "Player turn finished.";
    shared_ptr<GameEvent> event(new PlayerEvent(StateEventType::PlayerTurnEnded, currentPlayer));
    dispatcher.publish(event);
}

void World::onPlayerDefeated(int playerId) {
    players[playerId].setDead(true);
    PLOG(plog::info) << "Player" << playerId << " is defeated";
    shared_ptr<GameEvent> event(new PlayerEvent(StateEventType::PlayerDefeated, playerId));
    dispatcher.publish(event);
    checkPlayersWin();
}

void World::onPlayersWon(vector<int> &winners) {
    PLOG(plog::info) << "Players leading by " << winners[0] << " won!";
    shared_ptr<GameEvent> event(new PlayersWon(winners));
    dispatcher.publish(event);
}


/* ------------------------------- End of on... -------------------------------------- */


shared_ptr<Entity> World::createEntity(const Point &pos, const string &entityId, int playerId) {
    if (!isInside(pos)) {
        return shared_ptr<Entity>();
    }
    Tile &tile = getTile(pos);
    auto entity = EntityRepository::instance().createEntity(entityId, getNextObjectId());
    if (entity == nullptr) {
        PLOG_WARNING << "Unable to create entity named: " << entityId;
        return shared_ptr<Entity>();
    }
    entity->setPos(pos);
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

shared_ptr<Entity> World::removeEntitySimply(unsigned int objectId) {
    auto it = entityMap.find(objectId);
    if (it == entityMap.end()) {
        return shared_ptr<Entity>();
    }
    auto en = it->second;
    auto &pos = en->getPos();
    if (isInside(pos)) {
        auto &t = getTile(pos);
        if (t.getEntity() == en) {
            t.removeEntity();
        }
    }
    entityMap.erase(en->getObjectId());
    return en;
}

Player &World::getPlayer(int playerId) {
    return players[playerId];
}

bool World::isValidLivingPlayer(int playerId) {
    if (playerId < 0 || playerId >= playerCount) {
        return false;
    }
    return !getPlayer(playerId).isDead();
}

Player &World::getCurrentAsPlayer() {
    return players[currentPlayer];
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

void World::forEachEntitiesOf(int playerId, const function<void(int, int, shared_ptr<Entity> &)> &f) {
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

void World::initDistanceMap() const {
    if (distanceMap == nullptr) {
        distanceMap = initMatrix(width, height, PathRecord{Direction::NONE, INT32_MAX});
    } else {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                distanceMap[i][j].d = INT32_MAX;
                distanceMap[i][j].from = Direction::NONE;
            }
        }
    }
}

void World::computeDistanceMap(const Point &start, const Point &dest, shared_ptr<GameUnit> &unit) const {
    stack<Point> s;
    s.push(start);
    auto &directions = Point::directions();
    while (!s.empty()) {
        Point p = s.top();
        s.pop();
        int nDistance = distanceMap[p.x][p.y].d + unit->getTileRMP(getTile(p));

        for (int i = 0; i < directions.size(); i++) {
            const Point &d = directions[i];
            Point next = p + d;
            if (!isInside(next)) {
                continue;
            }
            auto &t = getTile(next);
            if (!t.canPassThrough(unit)) {
                continue;
            }
            if (nDistance >= distanceMap[next.x][next.y].d) {
                continue;
            }
            distanceMap[next.x][next.y].d = nDistance;
            distanceMap[next.x][next.y].from = (Direction) i;
            s.push(next);
        }
    }

}

int World::pathLength(const Point &start, const Point &dest, shared_ptr<GameUnit> &unit) const {
    initDistanceMap();
    distanceMap[start.x][start.y].d = 0;
    computeDistanceMap(start, dest, unit);
    return distanceMap[dest.x][dest.y].d;
}

vector<pair<Point, int>> World::findPath(const Point &start, const Point &dest, shared_ptr<GameUnit> &unit) const{
    initDistanceMap();
    distanceMap[start.x][start.y].d = 0;
    computeDistanceMap(start, dest, unit);
    if(distanceMap[dest.x][dest.y].from == Direction::NONE){
        return vector<pair<Point, int>>();
    }
    Point cur = dest;
    vector<pair<Point, int>> re;
    while (!(cur == start)) {
        auto& t = distanceMap[cur.x][cur.y];
        int dis = t.d;
        re.emplace_back(cur, dis);
        cur = cur - Point::directions()[(int) t.from];
    }
    re.emplace_back(start, 0);
    return re;
}

void World::serializeTo(ostream &output) {
    //general information
    output << className() << ' ';
    output << mapName << ' '
           << width << ' '
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
    string mapName;
    input >> mapName;
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
    world->configure();
//    world->
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

    // computes the visibility later
//    for (int i = 0; i < playerCount; i++) {
//        int vi = static_cast<int>(t.getVisibility(i));
//        out << vi << ' ';
//    }
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
//    for (int i = 0; i < playerCount; i++) {
//        int v;
//        input >> v;
//        t.visibility[i] = static_cast<Visibility>(v);
//    }
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
            if (en->getOwnerId() == playerId && en->getEntityName() == entityName) {
                return {i, j};
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

void World::checkPlayersWin() {
    // check for one player remaining
    vector<int> winners;
    for (Player &p : players) {
        if (p.isDead()) {
            continue;
        }
        if (winners.empty()) {
            winners.push_back(p.getPlayerId());
        } else {
            Player &rep = players[winners[0]];
            if (!rep.isAlly(p)) {
                return;//there is a player alive but he is not an ally.
            }
            winners.push_back(p.getPlayerId());
        }
    }
    onPlayersWon(winners);
}

void World::addEventListener(const EventListener &listener) {
    dispatcher.addListener(listener);
}

void World::publishEvent(shared_ptr<GameEvent> &event) {
    dispatcher.publish(event);
}

vector<const EntityInfo *> World::getAvailableEntitiesFor(int playerId) {
    auto &repo = EntityRepository::instance();
    Player &p = getPlayer(playerId);
    vector<const EntityInfo *> result;
    for (auto &entry : repo.getEntityMap()) {
        auto &enInfo = entry.second;
        if (enInfo.isBuyableByPlayer(p)) {
            result.push_back(&enInfo);
        }
    }
    return result;
}

vector<const Technology *> World::getResearchableTechFor(int playerId) {
    auto &repo = TechRepository::instance();
    Player &p = getPlayer(playerId);
    vector<const Technology *> result;
    for (auto &entry : repo.getTechnologies()) {
        auto &techInfo = entry.second;
        if (techInfo.isResearchable(p)) {
            result.push_back(&techInfo);
        }
    }
    return result;
}

void World::configure() {
    for (int i = 0; i < playerCount; i++) {
        updateVisibility(i);
    }
}

const string &World::getMapName() const {
    return mapName;
}

void World::setMapName(const string &mapName) {
    World::mapName = mapName;
}

void World::shrinkPlayerCount(int newCount) {
    if (newCount >= playerCount) {
        return;
    }
    newCount = max(1, newCount);
    playerCount = newCount;
    players.resize(newCount);
    currentPlayer %= playerCount;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Tile &t = getTile(i, j);
            if (!t.hasEntity()) {
                continue;
            }
            auto en = t.getEntity();
            if (en->getOwnerId() >= playerCount) {
                //remove it
                entityMap.erase(en->getObjectId());
                t.removeEntity();
            }
        }
    }
}


}