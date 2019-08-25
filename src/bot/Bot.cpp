//
// Created by liyicheng on 2019/8/10.
//

#include "Bot.h"
#include <chrono>
#include <memory>
#include <thread>
#include <core/messages/UnitMove.h>
#include <core/messages/SetPlayerData.h>
#include <core/messages/UnitAttack.h>
#include <core/messages/EntityPerform.h>
#include <core/messages/UnitBuy.h>
#include <plog/Log.h>
#include "config/EntityRepository.h"

namespace z2 {


void Bot::init(const shared_ptr<Server> &s, const shared_ptr<World> &w, int pid) {
    server = s.get();
    world = w.get();
    playerId = pid;
    randomEngine(); // make it random
    doInit();
}

void Bot::doBotTurn() {
    PLOG_INFO << "[Bot] Money = " << getPlayer().getGold();
}

void Bot::doInit() {

}

void Bot::makeOperation(const shared_ptr<GameMessage> &msg) {
    if (server) {
        server->acceptMessage(msg);
    }
}

int Bot::randomIntBetween(int downerInclusive, int upperExclusive) {
    std::uniform_int_distribution<int> dis(downerInclusive, upperExclusive - 1);
    return dis(randomEngine);
}

void Bot::sleepFor(long millisecond) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

Bot::Bot() {
    randomEngine();

}

BotDifficulty Bot::getDifficulty() {
    return difficulty;
}

Bot::Bot(BotDifficulty difficulty) : difficulty(difficulty) {}

bool Bot::moveToward(unsigned int unitId, const Point &dest) {
    return moveToward(world->getEntity(unitId), dest);
}

bool Bot::moveToward(const shared_ptr<Entity> &en, const Point &dest) {
    if (!world->isInside(dest)) {
        return false;
    }
    if (!en || en->getOwnerId() != playerId) {
        return false;
    }
    auto unit = dynamic_pointer_cast<GameUnit>(en);
    if (!unit) {
        return false;
    }
    PLOG_INFO << "[Bot] Moving <" << en->getEntityName() << "> toward (" << dest.x << "," << dest.y << ")";
    auto &from = unit->getPos();
    if (from == dest) {
        return true;
    }
    Point d;
    if (world->isOccupied(dest.x, dest.y)) {
        if (from.isAdjacentTo(dest)) {
            return true;
        }
        d = world->getNearestAdjacentEmptyPos(dest, unit);
    } else {
        d = dest;
    }
    if (!world->isInside(d)) {
        return false;
    }

    int moves = unit->getRemainingMoves();
    if (moves == 0) {
        return false;
    }
    auto path = world->findPath(from, d, unit);
    if (path.empty()) {
        return false;
    }
    for (auto &t : path) {
//        cout << "(" << t.first.x << "," << t.first.y << ")" << endl;
        if (t.second > moves) {
            continue;
        }
        if (world->getTile(t.first).isOccupied()) {
            if (!clearPos(t.first)) {
                continue;
            }
        }
//            PLOG_INFO << "[Bot] Moving <" << en->getEntityName() <<"> to (" << t.first.x << "," << t.first.y << ")";
        makeOperation(std::make_shared<UnitMove>(from, t.first));
        if (t.first == d) {
            return true;
        }
        break;
    }
    return false;
}

bool Bot::attack(const Point &from, const Point &dest) {
    if (!world->isInside(from) || !world->isInside(dest)) {
        return false;
    }
    auto en = dynamic_pointer_cast<BattleUnit>(world->getEntity(from));
    if (!en || en->getOwnerId() != playerId) {
        return false;
    }
    if (dynamic_pointer_cast<MeleeUnit>(en)) {
        if (!en->getPos().isAdjacentTo(dest)) {
            moveToward(en, dest);
        }
    } else if (dynamic_pointer_cast<RangeUnit>(en)) {
        if (!world->canAttack(from, dest, playerId)) {
            moveToward(en, dest);
        }
    }
    if (!world->canAttack(from, dest, playerId)) {
        return false;
    }
    PLOG_INFO << "[Bot] Attacking <" << en->getEntityName() << "> to (" << dest.x << "," << dest.y;
    shared_ptr<UnitAttack> msg(new UnitAttack(from, dest));
    makeOperation(msg);
}


const Player &Bot::getPlayer() {
    return world->getPlayer(playerId);
}

void Bot::setData(int gold, int techPoints) {
    makeOperation(make_shared<SetPlayerData>(playerId, gold, techPoints, getPlayer().getMaxTechPoints()));
}

vector<shared_ptr<Entity>> Bot::getFarmers() {
    vector<shared_ptr<Entity>> re;
    world->forEachEntitiesOf(playerId, [&re](int, int, const shared_ptr<Entity> &en) {
        if (en->getEntityName() == "Farmer") {
            re.push_back(en);
        }
    });
    return re;
}

shared_ptr<Entity> Bot::getConstructionBase() {
    shared_ptr<Entity> re;
    world->forEachEntitiesOf(playerId, [&re](int, int, const shared_ptr<Entity> &en) {
        if (en->getEntityName() == "ConstructionBase") {
            re = en;
        }
    });
    return re;
}

vector<shared_ptr<BattleUnit>> Bot::getBattleUnits() {
    vector<shared_ptr<BattleUnit>> re;
    world->forEachEntitiesOf(playerId, [&re](int, int, const shared_ptr<Entity> &en) {
        auto t = dynamic_pointer_cast<BattleUnit>(en);
        if (t) {
            re.push_back(t);
        }
    });
    return re;
}

vector<shared_ptr<Entity>> Bot::getEnemiesAround(const Point &pos, int d) {
    auto &player = getPlayer();
    vector<shared_ptr<Entity>> re;
    for (int x = pos.x - d; x <= pos.x + d; x++) {
        for (int y = pos.y - d; y <= pos.y + d; y++) {
            Point p(x, y);
            auto en = world->getEntity(p);
            if (!en) {
                continue;
            }
            auto owner = world->getOwner(en);
            if (owner && !player.isAlly(*owner)) {
                re.push_back(en);
            }
        }
    }
    return re;
}

void Bot::perform(const shared_ptr<Entity> &en) {
    auto &pos = en->getPos();
    if (world->canPerform(en->getPos(), playerId)) {
        shared_ptr<GameMessage> msg(new EntityPerform(pos));
        makeOperation(msg);
    }
}

bool Bot::clearPos(const Point &pos) {
    auto en = world->getEntity(pos);
    if (!en) {
        return true;
    }
    if (en->getOwnerId() != playerId) {
        return false;
    }
    Point p = world->getAdjacentEmptyPos(pos);
    if (world->isInside(p)) {
        if (world->canMove(pos, p, playerId)) {
            shared_ptr<UnitMove> msg(new UnitMove(pos, p));
            makeOperation(msg);
            return true;
        }
    }
    int remainingMoves = en->getRemainingMoves();
    for (int i = 2; i <= remainingMoves; i++) {
        for (int x = pos.x - i; x <= pos.x + i; x++) {
            int d = i - abs(pos.x - x);
            int y = pos.y + d;
            p = {x, y};
            if (world->canMove(pos, p, playerId)) {
                shared_ptr<UnitMove> msg(new UnitMove(pos, p));
                makeOperation(msg);
                return true;
            }
            y = pos.y - d;
            p = {x, y};
            if (world->canMove(pos, p, playerId)) {
                shared_ptr<UnitMove> msg(new UnitMove(pos, p));
                makeOperation(msg);
                return true;
            }
        }
    }

    return false;
}

vector<Point> Bot::findResources() {
    vector<Point> re;
    for (int x = 0; x < world->getWidth(); x++) {
        for (int y = 0; y < world->getHeight(); y++) {
            auto r = world->getTile(x, y).getResource();
            if (r == Resource::MINE || r == Resource::GEM) {
                re.emplace_back(x, y);
            }
        }
    }
    return re;
}

vector<shared_ptr<Entity>> Bot::findEnemyEntities(const string &enName) {
    vector<shared_ptr<Entity>> re;
    auto &player = getPlayer();
    for (int x = 0; x < world->getWidth(); x++) {
        for (int y = 0; y < world->getHeight(); y++) {
            auto en = world->getTile(x, y).getEntity();
            if (!en) {
                continue;
            }
            auto owner = world->getOwner(en);
            if (en->getEntityName() == enName && owner && !player.isAlly(*owner)) {
                re.push_back(en);
            }
        }
    }
    return re;
}

vector<shared_ptr<Entity>> Bot::findAllEnemies() {
    vector<shared_ptr<Entity>> re;
    auto &player = getPlayer();
    for (int x = 0; x < world->getWidth(); x++) {
        for (int y = 0; y < world->getHeight(); y++) {
            auto en = world->getTile(x, y).getEntity();
            if (!en) {
                continue;
            }
            auto owner = world->getOwner(en);
            if (owner && !player.isAlly(*owner)) {
                re.push_back(en);
            }
        }
    }
    return re;
}

shared_ptr<Entity> Bot::buyUnit(const Point &pos, const string &name) {
    PLOG_INFO << "[Bot] Buying: <" << name << "> at (" << pos.x << "," << pos.y << "). Money = "
              << getPlayer().getGold();
    if (world->getAdjacentEmptyTile(pos) == nullptr) {
        for (auto &d : Point::directions()) {
            Point p = pos + d;
            if (clearPos(p)) {
                break;
            }
        }
    }
    if (world->canBuy(playerId, pos, name)) {
        makeOperation(shared_ptr<GameMessage>(new UnitBuy(name, pos, playerId)));
        shared_ptr<Entity> unit;
        for (auto &d : Point::directions()) {
            Point t = pos + d;
            auto en = world->getEntity(t);
            if (en && en->getEntityName() == name) {
                if (!unit || unit->getObjectId() < en->getObjectId()) {
                    unit = en;
                }
            }
        }
        return unit;
    }
    return shared_ptr<Entity>();
}

shared_ptr<Entity> Bot::buyLatestBattleUnit(const Point &pos, bool buyRange) {
    auto chooses = world->getAvailableEntitiesFor(playerId);
    if (chooses.empty()) {
        return nullptr;
    }
    int price = 0;
    const EntityInfo *selected = nullptr;
    for (auto info : chooses) {
        auto &prop = info->getProperties();
        if (prop.getInt("attackStrength", 0) != 0) {
            if (buyRange) {
                if (prop.getInt("range", 0) == 0) {
                    continue;
                }
            } else {
                if (prop.getInt("range", 0) != 0) {
                    continue;
                }
            }
            if (prop.getInt("price", 0) > price) {
                selected = info;
            }
        }
    }
    if (selected == nullptr) {
        return nullptr;
    }
    return buyUnit(pos, selected->getIdentifier());
}

vector<shared_ptr<Entity>> Bot::getAllOwnedEntities() {
    vector<shared_ptr<Entity>> re;
    for (auto &it : world->getEntityMap()) {
        if (it.second->getOwnerId() == playerId) {
            re.push_back(it.second);
        }
    }
    return re;
}


Bot &Bot::operator=(const Bot &bot) = default;

Bot::Bot(const Bot &bot) = default;

Bot::~Bot() = default;

}