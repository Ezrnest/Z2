//
// Created by liyicheng on 2019/7/29.
//

#ifndef Z2_GAMEINITSETTING_H
#define Z2_GAMEINITSETTING_H
#include <memory>
#include <string>
#include <vector>
#include <bot/Bot.h>

using namespace std;
namespace z2 {
class GameMap;
class World;
class Server;
class LocalClient;
enum class PlayerType {
    LOCAL_PLAYER,
    BOT_PLAYER,
    REMOTE_PLAYER,
};
class PlayerSetting{
public:
    PlayerSetting(int playerId, int positionId, int groupId, PlayerType type);
    PlayerSetting();
    int playerId;
    int positionId;
    int groupId;
    int colorCode = 0;
    BotDifficulty diff = BotDifficulty::NONE;
    PlayerType type;
};

class GameInitSetting {
private:
    vector<PlayerSetting> players;
    shared_ptr<GameMap> map;
public:
    GameInitSetting(int playerCount, shared_ptr<GameMap> map);

    GameInitSetting(vector<PlayerSetting> players, shared_ptr<GameMap> map);


    int getPlayerCount() const;

    void setupPlayer(int idx,const PlayerSetting& s);

    bool isValidLocalSetting(bool isLocalGame);

    shared_ptr<World> buildWorld();

    /**
     * Builds a server for local game. Only local clients will be registered.
     */
    pair<shared_ptr<Server>,shared_ptr<LocalClient>> buildLocalGame();

    const vector<PlayerSetting> &getPlayers() const;

    const shared_ptr<GameMap> &getMap() const;
};

void initBot(shared_ptr<Server>& server,shared_ptr<World> & w, PlayerSetting& ps);

}


#endif //Z2_GAMEINITSETTING_H
