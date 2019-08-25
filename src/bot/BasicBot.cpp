/*
 * Created by liyicheng on 2019/8/23.
 */

#include "BasicBot.h"
#include "core/messages/EntityPerform.h"
#include "TaskGuard.h"
#include "TaskResearch.h"
#include "TaskMoveAttack.h"
#include <algorithm>
#include <memory>
#include <plog/Log.h>

namespace z2 {
namespace bot {


void BasicBot::doInit() {
    PLOG_INFO << "[BasicBot] Init";
    auto cb = getConstructionBase();
    if (cb) {
        conBase = cb->getPos();
    }
    research.reset(new TaskResearch(this));
}

void BasicBot::performMine(const Point &pos) {
    if (!world->canPerform(pos, playerId)) {
        return;
    }
    int prevGold = getPlayer().getGold();
    makeOperation(make_shared<EntityPerform>(pos));
//    world->performEntity(pos);
    int curGold = getPlayer().getGold();
    int increment = static_cast<int>((curGold - prevGold) * goldMultiplier);
    int newGold = prevGold + increment;
    setData(newGold, getPlayer().getTechPoints());
}

void BasicBot::perform(const shared_ptr<Entity> &en) {
    if (dynamic_pointer_cast<Farmer>(en)) {
        performMine(en->getPos());
    } else {
        Bot::perform(en);
    }
}

void BasicBot::makeOperation(const shared_ptr<GameMessage> &msg) {
    Bot::makeOperation(msg);
    sleepFor(operationInterval);
}

void BasicBot::doBasicProduction() {
    PLOG_INFO << "[Bot] Production";
    for (auto &tcr : farming) {
        if (!tcr->isFarmerAlive()) {
            auto f = buyUnit(conBase, "Farmer");
            if (!f) {
                break;
            }
            tcr->resetFarmer(f->getObjectId());
        }
        if (tcr->getGuardianCount() < minFarmerGuardianCount) {
            auto en = buyLatestBattleUnit(conBase, randomIntBetween(0, 2) == 0);
            if (!en) {
                break;
            }
            tcr->addGuardian(en->getObjectId());
        }
    }
    while (farming.size() < desiredFarmerCount) {
        if (!planForNewResource()) {
            break;
        }
    }

}

void BasicBot::doResearch() {
    PLOG_INFO << "[Bot] Research";
    if (!research) {
        research.reset(new TaskResearch(this));
    }
    research->perform();
}

void BasicBot::doCommandUnits() {
    doFarm();
    doCombat();
}

void BasicBot::doFarm() {
    PLOG_INFO << "[Bot] Farming";
    for (auto &tcr : farming) {
        while (tcr->getGuardianCount() < minFarmerGuardianCount && !extraBattleUnits.empty()) {
            auto tg = extraBattleUnits.back();
            extraBattleUnits.pop_back();
            tcr->addGuardian(tg->getEntityId());
        }
        tcr->perform();
    }

//    farming = Task::performAndFilter<TaskCaptureResource>(farming);
}

void BasicBot::launchAttackTo(EntityId enemy, int maxCount) {
    while (maxCount > 0 && !extraBattleUnits.empty()) {
        auto en = extraBattleUnits.back()->getEntityId();
        extraBattleUnits.pop_back();

        combating.push_back(shared_ptr<TaskMoveAttack>(new TaskMoveAttack(this, en, enemy)));
    }
}

bool BasicBot::launchAttackToFarmer() {
    auto enemyFarmers = findEnemyEntities("Farmer");
    if (enemyFarmers.empty()) {
        return false;
    }
    auto farmer = enemyFarmers[0];
    launchAttackTo(farmer->getObjectId(), extraBattleUnitCountToLaunchAttack);
}

bool BasicBot::launchAttackToBase() {
    auto enemyBases = findEnemyEntities("ConstructionBase");
    if (enemyBases.empty()) {
        return false;
    }
    auto base = enemyBases[0];
    launchAttackTo(base->getObjectId(), extraBattleUnits.size());
}

bool BasicBot::launchAttackToAll() {
    auto enemies = findAllEnemies();
    if (enemies.empty()) {
        return false;
    }
    auto en = enemies[0];
    launchAttackTo(en->getObjectId(), extraBattleUnits.size());
}

void BasicBot::doCombat() {
    PLOG_INFO << "[Bot] Combating";
    if (extraBattleUnits.size() >= extraBattleUnitCountToLaunchAttack) {
        if (launchAttackToFarmer()) {
            return;
        }
        if (launchAttackToBase()) {
            return;
        }
        launchAttackToAll();
    }
    for (auto &tg : combating) {
        tg->perform();
    }
    guarding = Task::performAndFilter(guarding);
    for (auto &tg : extraBattleUnits) {
        tg->perform();
    }
}

void BasicBot::refreshInfo() {
    vector<shared_ptr<EntityTask>> ncb;
    for (auto &tg : combating) {
        if (tg->entityDied()) {
            continue;
        }
        if (tg->isActive()) {
            ncb.push_back(tg);
        } else {
            extraBattleUnits.push_back(make_shared<TaskGuard>(this, tg->getEntityId()));
        }
    }
    combating = ncb;
    vector<shared_ptr<TaskGuard>> ngd;
    for (auto &tg : guarding) {
        if (tg->entityDied()) {
            continue;
        }
        if (tg->isActive()) {
            ngd.push_back(tg);
        } else {
            extraBattleUnits.push_back(make_shared<TaskGuard>(this, tg->getEntityId()));
        }
    }
    guarding = ngd;

    while (farming.size() > desiredFarmerCount) {
        auto tcr = farming.back();
        if (!tcr->isFarmerAlive() && tcr->getGuardianCount() == 0) {
            farming.pop_back();
        }
        PLOG_INFO << "[BasicBot] Cleared a farming task.";
    }

}

void BasicBot::doBotTurn() {
    Bot::doBotTurn();
    doCommandUnits();
    doBasicProduction();
    doResearch();
    doExtraProduction();
}

BasicBot::BasicBot(BotDifficulty difficulty, const string &name) : Bot(difficulty), name(name) {}

const string &BasicBot::getBotName() {
    return name;
}

Point BasicBot::findNextNearestResource() {
    auto res = findResources();
    if (res.empty()) {
        return {};
    }
    sort(res.begin(), res.end(), [this](const Point &p1, const Point &p2) {
        return p1.girdDistance(conBase) < p2.girdDistance(conBase);
    });
    for (auto &p : res) {
        bool used = false;
        for (auto &tcr : farming) {
            if (tcr->getTargetMine() == p) {
                used = true;
                break;
            }
        }
        if (!used) {
            return p;
        }
    }
    return {};
}

bool BasicBot::planForNewResource() {
    Point p = findNextNearestResource();
    PLOG_INFO << "[Bot] Next resource: (" << p.x << "," << p.y << ")";
    if (!world->isInside(p)) {
        return false;
    }
    auto f = buyUnit(conBase, "Farmer");
    if (!f) {
        return false;
    }
    farming.push_back(std::make_shared<TaskCaptureResource>(this, p, f->getObjectId()));
    return true;
}

void BasicBot::doExtraProduction() {
    int saveMoneyIdx = randomIntBetween(0, 100);
    if (saveMoneyIdx < 60) {
        return;
    }
    int expandIdx = randomIntBetween(0, 100);
    if (expandIdx < 40) {
        planForNewResource();
    } else {
        strengthenMilitaryForce();
    }
}

bool BasicBot::strengthenMilitaryForce() {
    auto en = buyLatestBattleUnit(conBase, randomIntBetween(0, 10) < 4);
    if (!en) {
        return false;
    }
    Point p = findRandomEmptyTile();
    if (!world->isInside(p)) {
        p = en->getPos();
    }
    int consolidateFarmingIdx = randomIntBetween(0, 100);
    if (consolidateFarmingIdx < 70) {
        consolidateFarming(en);
        return true;
    }
    extraBattleUnits.push_back(make_shared<TaskGuard>(this, en->getObjectId(), p));
    return true;
}

void BasicBot::consolidateFarming(const shared_ptr<Entity> &en) {
    bool captureGem = randomIntBetween(0, 100) < 25;
    shared_ptr<TaskCaptureResource> tcr;
    int minMeleeCount = 1000;
    for (auto &t : farming) {
        int count = t->getGuardianCount(); // effectively
        if (t->getResource() == Resource::GEM) {
            count -= extraGuardianForGem;
        }
        if (count < minMeleeCount) {
            tcr = t;
            minMeleeCount = count;
        }
    }
    if (tcr && minMeleeCount < maxFarmerGuardianCount) {
        tcr->addGuardian(en->getObjectId());
        return;
    }

}

Point BasicBot::findRandomEmptyTile(int d) {
    auto entities = getAllOwnedEntities();
    int xMin = world->getWidth();
    int yMin = world->getHeight();
    int xMax = -1;
    int yMax = -1;
    for (auto &en : entities) {
        auto &p = en->getPos();
        xMin = min(xMin, p.x);
        yMin = min(yMin, p.y);
        xMax = max(xMax, p.x);
        yMax = max(yMax, p.y);
    }
    xMin -= d;
    yMin -= d;
    xMax += d;
    yMax += d;
    for (int i = 0; i < 10; i++) {
        int x = randomIntBetween(xMin, xMax + 1);
        int y = randomIntBetween(yMin, yMax + 1);
        if (!world->isInside(x, y)) {
            continue;
        }
        if (world->isOccupied(x, y)) {
            continue;
        }
        return {x, y};
    }
    return {};
}


BasicBotProvider::BasicBotProvider(double goldMultiplier, const string &name) : goldMultiplier(goldMultiplier),
                                                                                name(name) {}

shared_ptr<Bot> BasicBotProvider::operator()(BotDifficulty difficulty) {
    return shared_ptr<Bot>(new BasicBot(difficulty, name));
}
}
}