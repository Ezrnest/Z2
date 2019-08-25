//
// Created by liyicheng on 2019/8/10.
//

#include <plog/Log.h>
#include <bot/Dummy.h>
#include "BotRepository.h"
#include <random>
#include "bot/BasicBot.h"

namespace z2 {


shared_ptr<Bot> BotRepository::getBot(BotDifficulty difficulty) {
    if (providers.find(difficulty) == providers.end()) {
        PLOG_WARNING << "No bot available for difficulty: " << (int) difficulty;
        return dummyProvider(difficulty);
    } else {
        auto &vec = providers[difficulty];
        randomEngine();
        uniform_int_distribution<int> distribution(0, vec.size() - 1);
        int select = distribution(randomEngine);
        return vec[select](difficulty);
    }
}

BotRepository::BotRepository() {
    dummyProvider = [](BotDifficulty d) {
        return shared_ptr<Bot>(new Dummy(d));
    };
    randomEngine();
    randomEngine();
}

void BotRepository::addProvider(const BotProvider &provider, BotDifficulty diff) {
    if (providers.find(diff) == providers.end()) {
        vector<BotProvider> vec;
        vec.push_back(provider);
        providers.insert(make_pair(diff, vec));
    } else {
        auto &vec = providers[diff];
        vec.push_back(provider);
    }
}

void addBasicBots(BotRepository &repository, BotDifficulty diff, const vector<string>& names) {
    double mul;
    switch (diff) {
        case BotDifficulty::NONE:
        case BotDifficulty::EASY:
            mul = 1.0;
            break;
        case BotDifficulty::NORMAL:
            mul = 1.8;
            break;
        case BotDifficulty::HARD:
            mul = 3;
            break;
    }
    for (auto &n : names) {
        bot::BasicBotProvider provider(mul, n);
        repository.addProvider(provider, diff);
    }
}

void addEasyBots(BotRepository &repository) {
    vector<string> names{
            "Bot_Pinkie", "Bot_Alice", "Bot_Tommy", "Bot_Tomato",
            "Bot_Bob", "Bot_Ez", "Bot_000", "Bot_Faye"
    };
    addBasicBots(repository, BotDifficulty::EASY, names);
}

void addNormalBots(BotRepository &repository) {
    vector<string> names{
            "Bot_Alory", "Bot_Joyce", "Bot_Jimmy", "Bot_Potato",
            "Bot_Richard", "Bot_Han", "Bot_Zhang", "Bot_Black"
    };
    addBasicBots(repository, BotDifficulty::NORMAL, names);
}

void addHardBots(BotRepository &repository) {
    vector<string> names{
            "Bot_Dezza", "Bot_TS", "Bot_Li", "Bot_Ez",
            "Bot_Void", "Bot_Kant", "Bot_Dash", "Bot_Crisp"
    };
    addBasicBots(repository, BotDifficulty::HARD, names);
}


void BotRepository::initBots() {
    auto &repo = instance();
    repo.registerBotClass<Dummy>(BotDifficulty::NONE);

//    BotProvider provider = [](z2::BotDifficulty diff) {
//        return shared_ptr<Bot>(new bot::BasicBot(diff, "Bot"));
//    };
//    repo.addProvider(provider, BotDifficulty::NONE);
//    repo.addProvider(provider, BotDifficulty::EASY);
    addEasyBots(repo);
    addNormalBots(repo);
    addHardBots(repo);
}

void BotRepository::initFromFolder(const ancono::File &dir) {

}


}