/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_COMMANDLINEGAMEGUI_H
#define Z2_COMMANDLINEGAMEGUI_H

#include "../core/GameGui.h"

namespace z2 {
class CommandLineGameGui : public GameGui{
public:
    void update() override;

    void onPlayerTurnStarted(int playerId) override;

    void onGameStarted() override;

    void onPlayerWin(int playerId) override;

    void printWorld();
};
}

#endif //Z2_COMMANDLINEGAMEGUI_H
