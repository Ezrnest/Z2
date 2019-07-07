/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_GAMEGUI_H
#define Z2_GAMEGUI_H

#include "Client.h"

namespace z2 {
class Client;

/**
 * Describes the gui interface for the game. This interface can be implemented in several ways,
 * such as CommandLineGameGui or QtGameGui.
 */
class GameGui {
protected:
    shared_ptr<Client> client;

public:
    virtual ~GameGui();

    /**
     * Sets the client to the game gui. Further user input will be dispatched to the
     * client(as controller).
     */
    void setControllerAndView(const shared_ptr<Client> &c);

    /**
     * Calls the game gui to update.
     */
    virtual void update() = 0;

    virtual void onPlayerTurnStarted(int playerId) = 0;

    virtual void onGameStarted() = 0;

    virtual void onPlayerWin(int playerId) = 0;
};

}

#endif //Z2_GAMEGUI_H
