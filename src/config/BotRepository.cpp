//
// Created by liyicheng on 2019/8/10.
//

#include <plog/Log.h>
#include <bot/Dummy.h>
#include "BotRepository.h"
#include <random>
namespace z2 {


shared_ptr<Bot> BotRepository::getBot(BotDifficulty difficulty) {
    if(providers.find(difficulty) == providers.end()){
        PLOG_WARNING << "No bot available for difficulty: " << (int)difficulty;
        return dummyProvider();
    }else{
        auto &vec = providers[difficulty];

        uniform_int_distribution<int> distribution(0, vec.size() - 1);
        int select = distribution(randomEngine);
        return vec[select]();
    }
}

BotRepository::BotRepository(){
    dummyProvider = [](){
        return shared_ptr<Bot>(new Dummy());
    };
}

void BotRepository::addProvider(const BotProvider &provider, BotDifficulty diff) {
    if(providers.find(diff) == providers.end()){
        vector<BotProvider> vec;
        vec.push_back(provider);
        providers.insert(make_pair(diff, vec));
    }else{
        auto &vec = providers[diff];
        vec.push_back(provider);
    }
}

void BotRepository::initBots() {
    auto& repo = instance();
    repo.registerBotClass<Dummy>(BotDifficulty::NONE);

}

void BotRepository::initFromFolder(const ancono::File &dir) {

}


}