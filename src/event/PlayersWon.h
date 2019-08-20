/*
 * Created by liyicheng on 2019/8/20.
 */

#ifndef Z2_PLAYERSWON_H
#define Z2_PLAYERSWON_H

#include <vector>
#include "StateEvent.h"

using namespace std;
namespace z2{
class PlayersWon :public StateEvent{
private:
    vector<int> playerIds;
public:
    explicit PlayersWon(const vector<int> &playerIds);

    const vector<int> &getPlayerIds() const;
};

}



#endif //Z2_PLAYERSWON_H
