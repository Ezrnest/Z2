//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_REGISTERPLAYER_H
#define Z2_REGISTERPLAYER_H

#include "ControlMessage.h"

namespace z2 {
class RegisterPlayer : public ControlMessage {
private:
    int playerId;
    string playerName;
public:
    RegisterPlayer(int playerId, string playerName);

    RegisterPlayer();

    int getPlayerId() const;

    const string &getPlayerName() const;

protected:
    void serializeData(ostream &output) override;

public:
    const string &getClassName() const override;


    void deserializeData(istream &input) override;
};
}

#endif //Z2_REGISTERPLAYER_H
