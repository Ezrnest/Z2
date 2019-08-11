//
// Created by liyicheng on 2019/8/10.
//

#ifndef Z2_BOTREPOSITORY_H
#define Z2_BOTREPOSITORY_H

#include <bot/Bot.h>
#include "RepositoryTemplate.h"
#include <functional>
#include <ctime>

using namespace std;
namespace z2 {

using BotProvider = function<shared_ptr<Bot>()>;

class BotRepository : public RepositoryTemplate<BotRepository> {
private:
    BotProvider dummyProvider;
    map<BotDifficulty, vector<BotProvider>> providers;

    default_random_engine randomEngine{1 + (unsigned int) time(nullptr)};

public:

    BotRepository();

    void addProvider(const BotProvider &provider, BotDifficulty d);


    /**
     * Register a bot class to the bot repository.
     */
    template<typename BotClass>
    void registerBotClass(BotDifficulty difficulty);

    /**
     * Gets a random bot of the given difficulty.
     */
    shared_ptr<Bot> getBot(BotDifficulty difficulty);

    static void initBots();

    void initFromFolder(const ancono::File &dir) override;

};


template<typename BotClass>
void BotRepository::registerBotClass(BotDifficulty difficulty) {
    static_assert(is_base_of<Bot,BotClass>::value, "The registered bot class must be subclass of Bot!");
    BotProvider provider = []() {
        return shared_ptr<Bot>(new BotClass());
    };
    addProvider(provider, difficulty);
}

}


#endif //Z2_BOTREPOSITORY_H
