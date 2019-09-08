//
// Created by chenzc18 on 2019/8/29.
//
#include "AlphaBot.h"
#include "core/messages/EntityPerform.h"
#include "TaskGuard.h"
#include "TaskResearch.h"
#include "TaskMoveAttack.h"
#include "TaskMine.h"
#include "world/Technology.h"
#include <algorithm>
#include <memory>
#include <plog/Log.h>
#include <core/messages/TechResearch.h>
#include <config/EntityRepository.h>
#include "config/GameConfiguration.h"
namespace z2 {
    namespace bot {
        void AlphaBot::doInit() {
            PLOG_INFO << "[AlphaBot] Init";
            auto cb = getConstructionBase();
            if (cb) {
                conBase = cb->getPos();
            }
            research.reset(new TaskResearch(this));
            //load all units
            auto farmers = getFarmers();
            for (auto &f : farmers) {
                assignFarmer(f);
            }
            auto units = getBattleUnits();
            for (auto &en : units) {
                assignStandBy(en->getObjectId());
            }
            auto& config = GameConfiguration::instance();
            operationInterval = config.getBotOperationInterval();
        }

        void AlphaBot::performMine(const Point &pos) {
            if (!world->canPerform(pos, playerId)) {
                return;
            }
            int prevGold = getPlayer().getGold();
            makeOperation(make_shared<EntityPerform>(pos));
            int curGold = getPlayer().getGold();
            int increment = static_cast<int>((curGold - prevGold) * goldMultiplier);
            int newGold = prevGold + increment;
            setData(newGold, getPlayer().getTechPoints());
        }

        void AlphaBot::perform(const shared_ptr<Entity> &en) {
            if (dynamic_pointer_cast<Farmer>(en)) {
                performMine(en->getPos());
            } else {
                Bot::perform(en);
            }
        }

        void AlphaBot::makeOperation(const shared_ptr<GameMessage> &msg) {
            Bot::makeOperation(msg);
            sleepFor(operationInterval);
        }

        void AlphaBot::doBasicProduction() {
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
            while (farming.size() < minFarmerCount) {
                if (!planForNewResource()) {
                    break;
                }
            }

        }

        void AlphaBot::doResearch() {
            PLOG_INFO << "[Bot] Research";
            auto techList = world->getResearchableTechFor(playerId);
            if (techList.empty()) {
                return;
            }
            auto it = min_element(techList.begin(), techList.end(), [](const Technology *t1, const Technology *t2) {
                return t1->getPrice() < t2->getPrice();
            });
            auto tech = (*it);
            if (tech->getPrice() > getPlayer().getGold()) {
                int income = computeIncomePerTurn();
                if (income > 0 && (double) tech->getPrice() / income <= maxNumOfTurnsToWaitForResearch) {
                    saveMoneyThisTurn = true;
                }
            } else {
                makeOperation(make_shared<TechResearch>(tech->getId(), playerId));
            }
        }

        void AlphaBot::doCommandUnits() {
            doFarm();
            doCombat();
        }

        void AlphaBot::doFarm() {
            PLOG_INFO << "[Bot] Farming";
            for (auto &tcr : farming) {
                while (tcr->getGuardianCount() < minFarmerGuardianCount && !extraBattleUnits.empty()) {
                    auto tg = extraBattleUnits.back();
                    extraBattleUnits.pop_back();
                    tcr->addGuardian(tg->getEntityId());
                }
                tcr->perform();
            }
        }

        void AlphaBot::launchAttackTo(EntityId enemy, int maxCount) {
            while (maxCount > 0 && !extraBattleUnits.empty()) {
                auto en = extraBattleUnits.back()->getEntityId();
                extraBattleUnits.pop_back();
                combating.push_back(shared_ptr<TaskMoveAttack>(new TaskMoveAttack(this, en, enemy)));
            }
        }

        bool AlphaBot::launchAttackToFarmer() {
            auto enemyFarmers = findEnemyEntities("Farmer");
            if (enemyFarmers.empty()) {
                return false;
            }
            auto farmer = enemyFarmers[0];
            launchAttackTo(farmer->getObjectId(), extraBattleUnitCountToLaunchAttack);
        }

        bool AlphaBot::launchAttackToBase() {
            auto enemyBases = findEnemyEntities("ConstructionBase");
            if (enemyBases.empty()) {
                return false;
            }
            auto base = enemyBases[0];
            launchAttackTo(base->getObjectId(), extraBattleUnits.size());
        }

        bool AlphaBot::launchAttackToAll() {
            auto enemies = findAllEnemies();
            if (enemies.empty()) {
                return false;
            }
            auto en = enemies[0];
            launchAttackTo(en->getObjectId(), extraBattleUnits.size());
        }

        void AlphaBot::doCombat() {
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
            if (!extraBattleUnits.empty()) {
                int defendIdx = randomIntBetween(0, 20);
                if (defendIdx < 10) {
                    Point pos = findRandomEmptyTile(0);
                    if (world->isInside(pos)) {
                        auto task = extraBattleUnits.front();
                        extraBattleUnits.erase(extraBattleUnits.begin());
                        guarding.push_back(make_shared<TaskGuard>(this, task->getEntityId(), pos));
                    }
                }
            }
            for (auto &tg : combating) {
                tg->perform();
            }
            for (auto &task : guarding) {
                task->perform();
            }
            for (auto &tg : extraBattleUnits) {
                tg->perform();
            }
        }

        void AlphaBot::refreshInfo() {
            vector<shared_ptr<EntityTask>> ncb;
            for (auto &tg : combating) {
                if (tg->entityDied()) {
                    continue;
                }
                if (tg->isActive()) {
                    ncb.push_back(tg);
                } else {
                    assignStandBy(tg->getEntityId());
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
                    assignStandBy(tg->getEntityId());
                }
            }
            guarding = ngd;

            while (farming.size() > minFarmerCount) {
                auto tcr = farming.back();
                if (!tcr->isFarmerAlive() && tcr->getGuardianCount() == 0) {
                    PLOG_INFO << "[AlphaBot] Cleared a farming task.";
                    farming.pop_back();
                }else{
                    break;
                }
            }
            saveMoneyThisTurn = false;
        }

        void AlphaBot::doBotTurn() {
            Bot::doBotTurn();
            refreshInfo();
            doCommandUnits();
            doBasicProduction();
            doResearch();
            doExtraProduction();
        }

        AlphaBot::AlphaBot(BotDifficulty difficulty, const string &name) : Bot(difficulty), name(name) {}

        const string &AlphaBot::getBotName() {
            return name;
        }

        Point AlphaBot::findNextNearestResource(const Point& start) {
            auto res = findResources();
            if (res.empty()) {
                return {};
            }
            sort(res.begin(), res.end(), [this,&start](const Point &p1, const Point &p2) {
                return p1.girdDistance(start) < p2.girdDistance(start);
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

        bool AlphaBot::planForNewResource() {
            auto f = buyUnit(conBase, "Farmer");
            if (!f) {
                return false;
            }
            return assignFarmer(f);
        }

        void AlphaBot::doExtraProduction() {
            int saveMoneyIdx = randomIntBetween(0, 100);
            if (saveMoneyIdx < 90 && saveMoneyThisTurn) {
                return;
            }
            int expandIdx = randomIntBetween(0, 100);
            if (expandIdx < 40) {
                planForNewResource();
            } else {
                strengthenMilitaryForce();
            }
        }

        bool AlphaBot::strengthenMilitaryForce() {
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

        void AlphaBot::consolidateFarming(const shared_ptr<Entity> &en) {
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

        Point AlphaBot::findRandomEmptyTile(int d) {
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

        int AlphaBot::computeIncomePerTurn() {
            int total = 0;
            for (auto &task : farming) {
                auto &mineTask = task->getMine();
                auto en = dynamic_pointer_cast<Farmer>(world->getEntity(mineTask->getEntityId()));
                if (!en) {
                    continue;
                }
                switch (task->getResource()) {
                    case Resource::NONE:
                        break;
                    case Resource::MINE: {
                        total += en->getGoldOfMine();
                        break;
                    }
                    case Resource::GEM: {
                        total += en->getGoldOfGem();
                        break;
                    }
                }
            }
            return total;
        }

        bool AlphaBot::assignFarmer(const shared_ptr<Entity> &f) {
            Point p = findNextNearestResource(f->getPos());
            PLOG_INFO << "[Bot] Next resource: (" << p.x << "," << p.y << ")";
            if (!world->isInside(p)) {
                return false;
            }
            farming.push_back(std::make_shared<TaskCaptureResource>(this, p, f->getObjectId()));
            return true;
        }

        bool AlphaBot::assignStandBy(EntityId unit){
            extraBattleUnits.push_back(make_shared<TaskGuard>(this,unit,unit));
        }

        AlphaBotProvider::AlphaBotProvider(const string &name) : name(name) {}

        shared_ptr<Bot> AlphaBotProvider::operator()(BotDifficulty difficulty) {
            auto tmpBot = new AlphaBot(difficulty, name);
            tmpBot->saveMoneyThisTurn = false;
            switch(difficulty)
            {
                case BotDifficulty::NONE:
                    break;
                case BotDifficulty::EASY:
                    tmpBot->minFarmerCount = 2;
                    tmpBot->minFarmerGuardianCount = 0;
                    tmpBot->maxFarmerGuardianCount = 2;
                    tmpBot->extraGuardianForGem = 0;
                    tmpBot->maxNumOfTurnsToWaitForResearch = 6;
                    tmpBot->extraBattleUnitCountToLaunchAttack = 4;
                    tmpBot->goldMultiplier = 1;
                    tmpBot->operationInterval = 1500;
                    break;
                case BotDifficulty::NORMAL:
                    tmpBot->minFarmerCount = 2;
                    tmpBot->minFarmerGuardianCount = 1;
                    tmpBot->maxFarmerGuardianCount = 3;
                    tmpBot->extraGuardianForGem = 1;
                    tmpBot->maxNumOfTurnsToWaitForResearch = 4;
                    tmpBot->extraBattleUnitCountToLaunchAttack = 3;
                    tmpBot->goldMultiplier = 2;
                    tmpBot->operationInterval = 1000;
                    break;
                case BotDifficulty::HARD:
                    tmpBot->minFarmerCount = 2;
                    tmpBot->minFarmerGuardianCount = 1;
                    tmpBot->maxFarmerGuardianCount = 5;
                    tmpBot->extraGuardianForGem = 2;
                    tmpBot->maxNumOfTurnsToWaitForResearch = 2;
                    tmpBot->extraBattleUnitCountToLaunchAttack = 2;
                    tmpBot->goldMultiplier = 3;
                    tmpBot->operationInterval = 500;
                    break;
            }
            return shared_ptr<Bot>(tmpBot);
        }
    }
}
